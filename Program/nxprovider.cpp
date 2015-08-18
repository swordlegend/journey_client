/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright © 2015 SYJourney                                               //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "nxprovider.h"

#define vectornode node::type::vector
#define bitmapnode node::type::bitmap
#define integernode node::type::integer
#define stringnode node::type::string

namespace data 
{
	map<string, map<char, pair<string, char>>> bodyactions;
	map<string, map<char, map<charlayers, map<string, vector2d>>>> bodyheadmap;

	map<cachemode, map<size_t, IWICBitmap*>> cache;

	nxprovider::nxprovider() 
	{
		cmlock = SRWLOCK_INIT;
	}

	void nxprovider::init(IWICImagingFactory* img_f, IDWriteFactory* w_f)
	{
		imagefactory = unique_ptr<IWICImagingFactory>(img_f);
		fontfactory = unique_ptr<IDWriteFactory>(w_f);

		fontfactory->CreateTextFormat(L"Times New Roman", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[DWF_LEFT]);
		fonts[DWF_LEFT]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		fonts[DWF_LEFT]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		fontfactory->CreateTextFormat(L"Times New Roman", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[DWF_CENTER]);
		fonts[DWF_CENTER]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		fonts[DWF_CENTER]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		fontfactory->CreateTextFormat(L"Times New Roman", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14.0f, L"en-us", &fonts[DWF_RIGHT]);
		fonts[DWF_RIGHT]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		fonts[DWF_RIGHT]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		fontfactory->CreateTextFormat(L"Times New Roman", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f, L"en-us", &fonts[DWF_LARGE]);
		fonts[DWF_LARGE]->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		fonts[DWF_LARGE]->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	nxprovider::~nxprovider() 
	{
		clearcache(CM_LOGIN);
		clearcache(CM_MAP);
		clearcache(CM_SYS);

		for (map<dwfonts, IDWriteTextFormat*>::iterator fnit = fonts.begin(); fnit != fonts.end(); ++fnit)
		{
			fnit->second->Release();
		}
	}

	IDWriteTextFormat* nxprovider::getfont(dwfonts type)
	{
		return fonts[type];
	}

	texture nxprovider::loadtexture(node source)
	{
		bitmap bmp = source.get_bitmap();

		vector2d dimension = vector2d(bmp.width(), bmp.height());
		vector2d origin;
		node originnode = source.resolve("origin");
		if (originnode.istype(vectornode))
			origin = vector2d(originnode.x(), originnode.y());
		else
			origin = vector2d();

		return texture(cachebitmap(bmp), origin, dimension);
	}

	animation nxprovider::loadanimation(node source)
	{
		map<byte, texture> textures;
		map<byte, short> delays;
		map<byte, pair<byte, byte>> alphablends;

		if (source.istype(bitmapnode))
		{
			textures[0] = loadtexture(source);
			alphablends[0] = pair<byte, byte>(255, 255);
		}
		else
		{
			for (node nodeit = source.begin(); nodeit != source.end(); nodeit++)
			{
				if (nodeit.istype(bitmapnode))
				{
					byte frame = static_cast<byte>(stoi(nodeit.name()));

					node delay = nodeit.resolve("delay");
					if (delay.istype(integernode))
						delays[frame] = static_cast<short>(delay.get_integer());
					else
						delays[frame] = 50;

					textures[frame] = loadtexture(nodeit);

					node a0_node = nodeit.resolve("a0");
					if (a0_node.istype(integernode))
					{
						byte a0 = static_cast<byte>(a0_node.get_integer());

						node a1_node = nodeit.resolve("a1");
						if (a1_node.istype(integernode))
							alphablends[frame] = pair<byte, byte>(a0, static_cast<byte>(a1_node.get_integer()));
						else
							alphablends[frame] = pair<byte, byte>(a0, 255 - a0);
					}
					else
					{
						alphablends[frame] = pair<byte, byte>(255, 255);
					}
				}
			}
		}

		if (textures.size() == 0)
			throw new runtime_error("the loading of a sprite failed.");

		return animation(textures, delays, alphablends);
	}

	charset nxprovider::loadcharset(node source)
	{
		map<char, texture> characters;

		for (node sub = source.begin(); sub != source.end(); sub++)
		{
			char c = sub.name()[0];
			if (c == '\\')
				c = '/';

			texture txt = loadtexture(sub);
			characters[c] = txt;
		}

		return charset(characters);
	}

	map<string, texture> nxprovider::loadbutton(node source)
	{
		map<string, texture> button;
		button["pressed"] = loadtexture(source.resolve("pressed/0"));
		button["mouseOver"] = loadtexture(source.resolve("mouseOver/0"));
		button["normal"] = loadtexture(source.resolve("normal/0"));
		button["disabled"] = loadtexture(source.resolve("disabled/0"));
		return button;
	}

	pair<vector<texture>, vector<texture>> nxprovider::loadnametag(node source)
	{
		pair<vector<texture>, vector<texture>> ret;
		ret.first.push_back(loadtexture(source.resolve("0/0")));
		ret.first.push_back(loadtexture(source.resolve("0/1")));
		ret.first.push_back(loadtexture(source.resolve("0/2")));
		ret.second.push_back(loadtexture(source.resolve("1/0")));
		ret.second.push_back(loadtexture(source.resolve("1/1")));
		ret.second.push_back(loadtexture(source.resolve("1/2")));
		return ret;
	}

	void nxprovider::setcmode(cachemode cm)
	{
		if (TryAcquireSRWLockExclusive(&cmlock))
		{
			cmode = cm;
		}
		else
		{
			Sleep(1);
			setcmode(cm);
		}
	}

	void nxprovider::unlock()
	{
		ReleaseSRWLockExclusive(&cmlock);
	}

	pair<cachemode, size_t> nxprovider::cachebitmap(bitmap texture)
	{
		if (cache[cmode][texture.id()])
			return make_pair(cmode, texture.id());

		IWICBitmap* temp = 0;

		imagefactory->CreateBitmapFromMemory(
			texture.width(), texture.height(),
			GUID_WICPixelFormat32bppBGRA,
			4 * texture.width(),
			texture.length(),
			(BYTE*)texture.data(),
			&temp);

		if (temp)
		{
			IWICFormatConverter* spConverter = 0;
			int result = imagefactory->CreateFormatConverter(&spConverter);
			if (SUCCEEDED(result))
			{
				spConverter->Initialize(temp, GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone, NULL, 0.f,
					WICBitmapPaletteTypeMedianCut);
				imagefactory->CreateBitmapFromSource(spConverter, WICBitmapNoCache, &cache[cmode][texture.id()]);
				spConverter->Release();
				temp->Release();
			}
		}

		if (cache[cmode][texture.id()])
		{
			return make_pair(cmode, texture.id());
		}
		else
		{
			return make_pair(cmode, -1);
		}
	}

	IWICBitmap* getbitmap(cachemode cm, size_t id)
	{
		return cache[cm][id];
	}

	void nxprovider::clearcache(cachemode cm)
	{
		for (map<size_t, IWICBitmap*>::iterator cit = cache[cm].begin(); cit != cache[cm].end(); cit++)
		{
			cit->second->Release();
		}
		cache[cm].clear();
	}

	npc nxprovider::loadnpc(int id)
	{
		setcmode(CM_MAP);
		nx::view_file("Npc");

		string fullname;
		string strid = to_string(id);
		char extend = 7 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		fullname.append(strid);
		node npcdata = nx::nodes["Npc"].resolve(fullname + ".img");

		map<string, animation> textures;
		map<string, vector<string>> linenames;
		for (node npcnode = npcdata.begin(); npcnode != npcdata.end(); npcnode++)
		{
			string state = npcnode.name();
			if (state == "info")
			{
				node speak = npcnode.resolve("speak");
				if (speak.size() > 0)
				{
					for (node speaknode = speak.begin(); speaknode != speak.end(); speaknode++)
					{
						linenames[state].push_back(speaknode.get_string());
					}
				}
			}
			else
			{
				textures[state] = loadanimation(npcnode);
				node speak = npcnode.resolve("speak");
				if (speak.size() > 0)
				{
					for (node speaknode = speak.begin(); speaknode != speak.end(); speaknode++)
					{
						linenames[state].push_back(speaknode.get_string());
					}
				}
			}
		}

		nx::unview_file("Npc");
		nx::view_file("String");

		node stringdata = nx::nodes["String"].resolve("Npc.img/" + strid);

		string name = "";
		string func = "";

		node namenode = stringdata.resolve("name");
		if (namenode.data_type() == node::type::string)
			name = namenode.get_string();

		node funcnode = stringdata.resolve("func");
		if (funcnode.data_type() == node::type::string)
			func = funcnode.get_string();

		map<string, vector<string>> lines;
		for (map<string, vector<string>>::iterator stit = linenames.begin(); stit != linenames.end(); stit++)
		{
			string state = stit->first;
			vector<string> names = stit->second;
			for (vector<string>::iterator nit = names.begin(); nit != names.end(); nit++)
			{
				string line = stringdata.resolve(*nit).get_string();
				lines[state].push_back(line);
			}
		}

		nx::unview_file("String");
		unlock();
		return npc(textures, lines, name, func);
	}

	mob nxprovider::loadmob(int id)
	{
		setcmode(CM_MAP);
		nx::view_file("Mob");

		string fullname;
		string strid = to_string(id);
		char extend = 7 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		fullname.append(strid);
		node mobdata = nx::nodes["Mob"].resolve(fullname + ".img");

		map<string, animation> textures;
		short level;
		short speed;

		for (node subnode = mobdata.begin(); subnode != mobdata.end(); ++subnode)
		{
			string state = subnode.name();

			if (state == "info")
			{
				level = static_cast<short>(subnode.resolve("level").get_integer());
				speed = static_cast<short>(subnode.resolve("level").get_integer());
			}
			else
			{
				textures[state] = loadanimation(subnode);
			}
		}

		nx::unview_file("Mob");
		nx::view_file("String");

		node stringdata = nx::nodes["String"].resolve("Mob.img/" + strid);

		string name;

		node namenode = stringdata.resolve("name");
		if (namenode.istype(stringnode))
			name = namenode.get_string();
		else
			name = "";

		nx::unview_file("String");
		unlock();
		return mob(textures, name, level, speed);
	}

	maplemap nxprovider::loadmap(int id)
	{
		setcmode(CM_MAP);
		nx::view_file("Map");

		string fullname;
		string strid = to_string(id);
		char extend = 9 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		node mapdata = nx::nodes["Map"].resolve("Map/Map" + to_string(id / 100000000) + "/" + fullname.append(strid) + ".img");

		map<char, map<char, vector<sprite>>> objlayers;
		map<char, map<char, vector<sprite>>> tilelayers;
		vector<background> backgrounds;
		vector<background> foregrounds;
		map<char, portal> portals;
		map<short, struct foothold> footholds;
		string mapdesc;
		string mapname;
		string streetname;

		node infonode = mapdata.resolve("info");

		vector2d mapwalls = vector2d(infonode.resolve("VRLeft").get_integer(), infonode.resolve("VRRight").get_integer());
		vector2d mapborders = vector2d(infonode.resolve("VRTop").get_integer(), infonode.resolve("VRBottom").get_integer());
		string bgm = infonode.resolve("bgm").get_string();
		bool cloud = infonode.resolve("cloud").get_bool();
		int fieldlimit = static_cast<int>(infonode.resolve("fieldLimit").get_integer());
		bool hideminimap = infonode.resolve("hideMinimap").get_bool();
		string mapmark = infonode.resolve("mapMark").get_string();
		bool swim = infonode.resolve("swim").get_bool();
		bool town = infonode.resolve("town").get_bool();

		node bgnodes = mapdata.resolve("back");

		nx::view_file("Back");
		node back = nx::nodes["Back"].resolve("Back/");

		for (node backnode = bgnodes.begin(); backnode != bgnodes.end(); backnode++)
		{
			string bS = backnode.resolve("bS").get_string();
			if (!bS.empty())
			{
				char layer = static_cast<char>(stoi(backnode.name()));
				bgtype type = static_cast<bgtype>(backnode.resolve("type").get_integer());
				bool ani = backnode.resolve("ani").get_bool();
				bool f = backnode.resolve("f").get_bool();

				node spritenode = back.resolve(bS + ".img/" + ((ani) ? "ani/" : "back/") + to_string(backnode.resolve("no").get_integer()));

				vector2d pos = vector2d(backnode.resolve("x").get_integer(), backnode.resolve("y").get_integer());
				vector2d rpos = vector2d(backnode.resolve("rx").get_integer(), backnode.resolve("ry").get_integer());
				vector2d cpos = vector2d(backnode.resolve("cx").get_integer(), backnode.resolve("cy").get_integer());
				byte alpha = static_cast<byte>(backnode.resolve("a").get_integer());

				background bgobj = background(loadanimation(spritenode), type, f, pos, rpos, cpos, mapwalls, mapborders, alpha);

				if (backnode.resolve("front").get_bool())
					foregrounds.push_back(bgobj);
				else
					backgrounds.push_back(bgobj);
			}
		}
		nx::unview_file("Back");

		node portalnodes = mapdata.resolve("portal");
		for (node portalnd = portalnodes.begin(); portalnd != portalnodes.end(); portalnd++)
		{
			char pid = static_cast<char>(stoi(portalnd.name()));

			portaltype ptype = static_cast<portaltype>(portalnd.resolve("pt").get_integer());
			string pname = portalnd.resolve("pn").get_string();
			vector2d ppos = vector2d(static_cast<int>(portalnd.resolve("x").get_integer()), static_cast<int>(portalnd.resolve("y").get_integer()));
			int targetid = static_cast<int>(portalnd.resolve("tm").get_integer());
			string targetpname = portalnd.resolve("tn").get_string();

			animation anim;

			node pnode = nx::nodes["Map"].resolve("MapHelper.img/portal/game");
			switch (ptype)
			{
			case PT_REGULAR:
				anim = loadanimation(pnode.resolve("pv"));
				break;
			case PT_HIDDEN:
			case PT_SCRIPTED_HIDDEN:
				anim = loadanimation(pnode.resolve("ph/default/portalContinue"));
				break;
			}

			portals[pid] = portal(ptype, pname, targetid, targetid == id, targetpname, anim, ppos);
		}

		node fhnodes = mapdata.resolve("foothold");
		for (node basef = fhnodes.begin(); basef != fhnodes.end(); basef++)
		{
			for (node midf = basef.begin(); midf != basef.end(); midf++)
			{
				for (node lastf = midf.begin(); lastf != midf.end(); lastf++)
				{
					short fid = (short)stoi(lastf.name());
					struct foothold fh;
					fh.prev = (short)lastf.resolve("prev").get_integer();
					fh.next = (short)lastf.resolve("next").get_integer();
					fh.horizontal = vector2d((int)lastf.resolve("x1").get_integer(), (int)lastf.resolve("x2").get_integer());
					fh.vertical = vector2d((int)lastf.resolve("y1").get_integer(), (int)lastf.resolve("y2").get_integer());
					footholds[fid] = fh;
				}
			}
		}


		nx::view_file("Obj");
		nx::view_file("Tile");

		for (int i = 0; i < 8; i++)
		{
			string layer = to_string(i);
			node nodeit = mapdata.resolve(layer);

			string tS = nodeit.resolve("info/tS").get_string();
			for (node layernode = nodeit.begin(); layernode != nodeit.end(); layernode++)
			{
				if (layernode.name() == "obj")
				{
					for (node objnode = layernode.begin(); objnode != layernode.end(); objnode++)
					{
						vector2d position = vector2d(
							static_cast<int>(objnode.resolve("x").get_integer()),
							static_cast<int>(objnode.resolve("y").get_integer()));
						int z = static_cast<int>(objnode.resolve("z").get_integer());
						node bmpnode = nx::nodes["Obj"].resolve("Obj/" + objnode.resolve("oS").get_string() + ".img/" + objnode.resolve("l0").get_string() + "/" + objnode.resolve("l1").get_string() + "/" + objnode.resolve("l2").get_string());
						objlayers[i][z].push_back(sprite(loadanimation(bmpnode), position, true, objnode.resolve("f").get_bool()));
					}
				}
				else if (layernode.name() == "tile")
				{
					for (node tilenode = layernode.begin(); tilenode != layernode.end(); tilenode++)
					{
						vector2d position = vector2d(
							static_cast<int>(tilenode.resolve("x").get_integer()),
							static_cast<int>(tilenode.resolve("y").get_integer()));
						int zM = static_cast<int>(tilenode.resolve("zM").get_integer());
						node bmpnode = nx::nodes["Tile"].resolve("Tile/" + tS + ".img/" + tilenode.resolve("u").get_string() + "/" + to_string(tilenode.resolve("no").get_integer()));
						tilelayers[i][zM].push_back(sprite(loadanimation(bmpnode), position));
					}
				}
			}
		}

		nx::unview_file("Map");
		nx::unview_file("Obj");
		nx::unview_file("Tile");
		unlock();
		return maplemap(id, tilelayers, objlayers, backgrounds, foregrounds, portals, footholds, fieldlimit, cloud, bgm, mapdesc, mapname, streetname, mapmark, swim, town, hideminimap, mapwalls, mapborders);
	}

	void nxprovider::initbodyinfo()
	{
		node bodynode = nx::nodes["Character"].resolve("00002000.img");
		node headnode = nx::nodes["Character"].resolve("00012000.img");

		for (node animnode = bodynode.begin(); animnode != bodynode.end(); animnode++)
		{
			string state = animnode.name();

			if (animnode.resolve("0").size() > 0)
			{
				for (node framenode = animnode.begin(); framenode != animnode.end(); framenode++)
				{
					char frame = stoi(framenode.name());

					node delaynode = framenode.resolve("delay");
					if (delaynode.istype(integernode))
						bodydelays[state][frame] = static_cast<short>(delaynode.get_integer());
					else
						bodydelays[state][frame] = 50;

					node actionnode = framenode.resolve("action");
					if (actionnode.istype(stringnode))
					{
						char actframe = frame;
						node actframenode = framenode.resolve("frame");

						if (actframenode.istype(integernode))
							actframe = static_cast<char>(actframenode.get_integer());

						bodyactions[state][frame] = make_pair(actionnode.get_string(), actframe);
					}
					else
					{
						for (node partnode = framenode.begin(); partnode != framenode.end(); partnode++)
						{
							string part = partnode.name();
							if (part != "delay" && part != "face")
							{
								charlayers z;
								string zs = partnode.resolve("z").get_string();
								if (zs == "armOverHair")
									z = CL_ARMOHAIR;
								else if (zs == "handOverHair")
									z = CL_RHAND;
								else if (zs == "body")
									z = CL_BODY;
								else if (zs == "handBelowWeapon")
									z = CL_LHAND;
								else if (zs == "arm")
									z = CL_ARM;
								else
									continue;

								node map = partnode.resolve("map");
								for (node mapnode = map.begin(); mapnode != map.end(); mapnode++)
								{
									bodyheadmap[state][frame][z][mapnode.name()] = vector2d(mapnode.x(), mapnode.y());
								}
							}
						}

						node headsfnode = headnode.resolve(state + "/" + to_string(frame) + "/head");

						node map = headsfnode.resolve("map");
						for (node mapnode = map.begin(); mapnode != map.end(); mapnode++)
						{
							bodyheadmap[state][frame][CL_HEAD][mapnode.name()] = vector2d(mapnode.x(), mapnode.y());
						}
					}
				}
			}
		}
	}

	void nxprovider::initfaceinfo()
	{
		node facenode = nx::nodes["Character"].resolve("Face/00020000.img");

		for (node expnode = facenode.begin(); expnode != facenode.end(); expnode++)
		{
			string state = expnode.name();

			if (state == "default")
			{
				facedelays[state][0] = 2500;
			}
			else if (state != "info")
			{
				for (node framenode = expnode.begin(); framenode != expnode.end(); framenode++)
				{
					string framename = framenode.name();

					if (framename == "delay")
					{
						facedelays[state][0] = static_cast<short>(framenode.get_integer());
					}
					else
					{
						char frame = static_cast<char>(stoi(framenode.name()));

						node delay = framenode.resolve("delay");
						if (delay.istype(integernode))
							facedelays[state][frame] = static_cast<short>(delay.get_integer());
						else
							facedelays[state][frame] = 2500;
					}
				}
			}
		}
	}

	void nxprovider::addbodytype(char skin)
	{
		string sk;
		if (skin < 10)
		{
			sk.append("0");
		}
		sk.append(to_string(skin));
		node bodynode = nx::nodes["Character"].resolve("000020" + sk + ".img");
		node headnode = nx::nodes["Character"].resolve("000120" + sk + ".img");

		charsprites body;

		for (node animnode = bodynode.begin(); animnode != bodynode.end(); animnode++)
		{
			string state = animnode.name();

			if (animnode.resolve("0").size() != 0)
			{
				for (node framenode = animnode.begin(); framenode != animnode.end(); framenode++)
				{
					char frame = stoi(framenode.name());

					for (node partnode = framenode.begin(); partnode != framenode.end(); partnode++)
					{
						string part = partnode.name();

						if (part != "delay" && part != "face")
						{
							charlayers z;
							string zs = partnode.resolve("z").get_string();
							if (zs == "armOverHair")
								z = CL_ARMOHAIR;
							else if (zs == "handOverHair")
								z = CL_RHAND;
							else if (zs == "body")
								z = CL_BODY;
							else if (zs == "handBelowWeapon")
								z = CL_LHAND;
							else if (zs == "arm")
								z = CL_ARM;
							else
								continue;

							vector2d shf = bodyheadmap[state][frame][CL_BODY]["navel"] - bodyheadmap[state][frame][z]["navel"];

							body.textures[state][frame][z] = loadtexture(partnode);
							body.textures[state][frame][z].shift(shf);
						}

						node headsfnode = headnode.resolve(state + "/" + to_string(frame) + "/head");
						if (headsfnode.istype(bitmapnode))
						{
							vector2d shf = bodyheadmap[state][frame][CL_BODY]["neck"] - bodyheadmap[state][frame][CL_HEAD]["neck"];

							body.textures[state][frame][CL_HEAD] = loadtexture(headsfnode);
							body.textures[state][frame][CL_HEAD].shift(shf);
						}
					}
				}
			}
		}

		bodytypes[skin] = body;
	}

	void nxprovider::addhairstyle(int hairid)
	{
		node hairnode = nx::nodes["Character"].resolve("Hair/000" + to_string(hairid) + ".img");

		charsprites hair;

		for (node statenode = hairnode.begin(); statenode != hairnode.end(); statenode++)
		{
			string state = statenode.name();

			if (state == "default" || state == "backDefault" || state == "info")
				continue;

			for (node framenode = statenode.begin(); framenode != statenode.end(); framenode++)
			{
				char frame = static_cast<char>(stoi(framenode.name()));

				for (node partnode = framenode.begin(); partnode != framenode.end(); partnode++)
				{
					string part = partnode.name();
					charlayers z;

					if (part == "hair")
						z = CL_HAIR;
					else if (part == "hairBelowBody")
						z = CL_HAIRBBODY;
					else if (part == "hairOverHead")
						z = CL_HAIROHEAD;
					else if (part == "hairShade")
						z = CL_HAIRSHADE;

					if (partnode.istype(bitmapnode))
					{
						vector2d brow = vector2d();
						node brownode = partnode.resolve("map/brow");
						if (brownode.istype(vectornode))
							brow = vector2d(static_cast<int>(brownode.x()), static_cast<int>(brownode.y()));

						vector2d shf = bodyheadmap[state][frame][CL_HEAD]["brow"] - bodyheadmap[state][frame][CL_HEAD]["neck"] + bodyheadmap[state][frame][CL_BODY]["neck"] - brow;

						hair.textures[state][frame][z] = loadtexture(partnode);
						hair.textures[state][frame][z].shift(shf);
					}
				}
			}
		}

		hairstyles[hairid] = hair;
	}

	void nxprovider::addface(int faceid)
	{
		node facenode = nx::nodes["Character"].resolve("Face/000" + to_string(faceid) + ".img");

		charsprites face;

		for (node expnode = facenode.begin(); expnode != facenode.end(); expnode++)
		{
			string state = expnode.name();

			if (state == "default")
			{
				node bmpnode = expnode.resolve("face");

				node brow = bmpnode.resolve("map/brow");

				face.textures[state][0][CL_FACE] = loadtexture(bmpnode);
				face.textures[state][0][CL_FACE].shift(vector2d(-brow.x(), -brow.y()));
				facedelays[state][0] = 2500;
			}
			else if (state != "info")
			{
				for (node framenode = expnode.begin(); framenode != expnode.end(); framenode++)
				{
					string framename = framenode.name();

					if (framename != "delay")
					{
						char frame = static_cast<char>(stoi(framenode.name()));

						node bmpnode = framenode.resolve("face");

						if (bmpnode.istype(bitmapnode))
						{
							node brow = bmpnode.resolve("map/brow");
							face.textures[state][frame][CL_FACE] = loadtexture(bmpnode);
							face.textures[state][frame][CL_FACE].shift(vector2d(-brow.x(), -brow.y()));

							node delay = framenode.resolve("delay");
							if (delay.istype(integernode))
								facedelays[state][frame] = static_cast<short>(delay.get_integer());
							else
								facedelays[state][frame] = 2500;
						}
					}
					else
					{
						facedelays[state][0] = static_cast<short>(framenode.get_integer());
					}
				}
			}
		}

		faces[faceid] = face;
	}

	void nxprovider::addclothes(int equipid)
	{
		charlayers category;
		string catname;
		int prefix = equipid / 10000;

		switch (prefix)
		{
		case 100:
			category = CL_HAT;
			catname = "Cap";
			break;
		case 101:
			category = CL_FACEACC;
			catname = "Accessory";
			break;
		case 102:
			category = CL_EYEACC;
			catname = "Accessory";
			break;
		case 103:
			category = CL_EARRINGS;
			catname = "Accessory";
			break;
		case 104:
			category = CL_TOP;
			catname = "Coat";
			break;
		case 105:
			category = CL_MAIL;
			catname = "Longcoat";
			break;
		case 106:
			category = CL_PANTS;
			catname = "Pants";
			break;
		case 107:
			category = CL_SHOES;
			catname = "Shoes";
			break;
		case 108:
			category = CL_GLOVE;
			catname = "Glove";
			break;
		case 109:
			category = CL_SHIELD;
			catname = "Shield";
			break;
		case 110:
			category = CL_CAPE;
			catname = "Cape";
			break;
		default:
			if (prefix >= 130 && prefix <= 170)
			{
				category = CL_WEAPON;
				catname = "Weapon";
			}
			else
			{
				return;
			}
		}

		node equipnode = nx::nodes["Character"].resolve(catname + "/0" + to_string(equipid) + ".img");

		charsprites cloth;

		for (node statenode = equipnode.begin(); statenode != equipnode.end(); statenode++)
		{
			string state = statenode.name();

			if (state == "default" || state == "backDefault" || state == "info")
				continue;

			for (node framenode = statenode.begin(); framenode != statenode.end(); framenode++)
			{
				char frame = static_cast<char>(stoi(framenode.name()));

				for (node partnode = framenode.begin(); partnode != framenode.end(); partnode++)
				{
					string part = partnode.name();
					
					if (partnode.istype(bitmapnode))
					{
						charlayers z = category;
						if (part == "mailArm")
							z = CL_MAILARM;

						string zs = partnode.resolve("z").get_string();
						if (z == CL_WEAPON)
						{
							if (zs == "weaponOverHand")
								z = CL_WEAPONOHAND;
							else if (zs == "weaponOverGlove")
								z = CL_WEAPONOGLOVE;
						}


						vector<string> parentpos;
						vector<vector2d> shiftmap;
						node map = partnode.resolve("map");
						for (node mapnode = map.begin(); mapnode != map.end(); mapnode++)
						{
							if (mapnode.data_type() == node::type::vector)
							{
								parentpos.push_back(mapnode.name());
								shiftmap.push_back(vector2d(mapnode.x(), mapnode.y()));
							}
						}

						vector2d shf;

						switch (z)
						{
						case CL_TOP:
						case CL_PANTS:
						case CL_CAPE:
						case CL_SHOES:
						case CL_MAIL:
						case CL_MAILARM:
						case CL_GLOVE:
							shf = bodyheadmap[state][frame][CL_BODY]["navel"] - shiftmap[0];
							break;
						case CL_HAT:
							shf = bodyheadmap[state][frame][CL_HEAD]["brow"] - bodyheadmap[state][frame][CL_HEAD]["neck"] + bodyheadmap[state][frame][CL_BODY]["neck"] - shiftmap[0];
							break;
						case CL_WEAPON:
						case CL_WEAPONOHAND:
						case CL_WEAPONOGLOVE:
							for (vector<string>::iterator it = parentpos.begin(); it != parentpos.end(); it++)
							{
								if (!it->compare("hand"))
								{
									if (bodyheadmap[state][frame].count(CL_ARM))
										shf = shf + bodyheadmap[state][frame][CL_ARM]["hand"] - bodyheadmap[state][frame][CL_ARM]["navel"];
									else if (bodyheadmap[state][frame].count(CL_ARMOHAIR))
										shf = shf + bodyheadmap[state][frame][CL_ARMOHAIR]["hand"] - bodyheadmap[state][frame][CL_ARMOHAIR]["navel"];
								}
								else if (!it->compare("handMove"))
									shf = shf + bodyheadmap[state][frame][CL_LHAND]["handMove"];
							}
							shf = shf + bodyheadmap[state][frame][CL_BODY]["navel"] - shiftmap[0];
							break;
						}

						cloth.textures[state][frame][z] = loadtexture(partnode);
						cloth.textures[state][frame][z].shift(shf);
					}
				}
			}
		}

		clothes[equipid] = cloth;
	}

	void nxprovider::loadcharlook(maplelook* plook)
	{
		charsprites look;

		if (bodytypes.size() == 0)
		{
			initbodyinfo();
		}

		char skin = plook->skin;
		int hair = plook->hairid;
		int face = plook->faceid;

		if (!bodytypes.count(skin))
		{
			addbodytype(skin);
		}
		look = bodytypes[skin];

		if (!hairstyles.count(hair))
		{
			addhairstyle(hair);
		}
		look.insert(hairstyles[hair]);

		if (!faces.count(face))
		{
			addface(face);
		}
		look.textures.insert(faces[face].textures.begin(), faces[face].textures.end());

		for (map<char, int>::iterator equip = plook->equips.begin(); equip != plook->equips.end(); equip++)
		{
			int equipid = equip->second;

			if (equipid != 0)
			{
				if (!clothes.count(equipid))
				{
					addclothes(equipid);
				}
				look.insert(clothes[equipid]);
			}
		}

		plook->addsprites(look, bodydelays, facedelays);
	}
}