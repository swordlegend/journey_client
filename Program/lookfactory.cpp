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
#pragma once
#include "lookfactory.h"
#include "nxfile.h"

namespace program
{
	void lookfactory::initbodyinfo()
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

	void lookfactory::initfaceinfo()
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

	void lookfactory::addbodytype(char skin)
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

							body.textures[state][frame][z] = texture(partnode);
							body.textures[state][frame][z].shift(shf);
						}

						node headsfnode = headnode.resolve(state + "/" + to_string(frame) + "/head");
						if (headsfnode.istype(bitmapnode))
						{
							vector2d shf = bodyheadmap[state][frame][CL_BODY]["neck"] - bodyheadmap[state][frame][CL_HEAD]["neck"];

							body.textures[state][frame][CL_HEAD] = texture(headsfnode);
							body.textures[state][frame][CL_HEAD].shift(shf);
						}
					}
				}
			}
		}

		bodytypes[skin] = body;
	}

	void lookfactory::addhairstyle(int hairid)
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

						hair.textures[state][frame][z] = texture(partnode);
						hair.textures[state][frame][z].shift(shf);
					}
				}
			}
		}

		hairstyles[hairid] = hair;
	}

	void lookfactory::addface(int faceid)
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

				face.textures[state][0][CL_FACE] = texture(bmpnode);
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
							face.textures[state][frame][CL_FACE] = texture(bmpnode);
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

	void lookfactory::addclothes(int equipid)
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

						cloth.textures[state][frame][z] = texture(partnode);
						cloth.textures[state][frame][z].shift(shf);
					}
				}
			}
		}

		clothes[equipid] = cloth;
	}

	void lookfactory::loadcharlook(maplelook* plook)
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

		plook->addsprites(look, bodydelays, facedelays, bodyactions, bodyheadmap);
	}
}
