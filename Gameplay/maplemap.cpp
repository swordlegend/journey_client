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
#include "maplemap.h"
#include "collision.h"

namespace gameplay
{
	maplemap::maplemap(int id, map<char, map<char, vector<sprite>>> t, map<char, map<char, vector<sprite>>> o, vector<background> b, vector<background> f, map<char, portal> p, map<short, fthold> fl, int fldl, bool cl, string bm, string md, string mn, string sn, string mm, bool s, bool tw, bool hm, vector2d wll, vector2d bnd)
	{
		mapid = id;
		tilelayers = t;
		objlayers = o;
		backgrounds = b;
		foregrounds = f;
		portals = p;
		footholds = fl;
		fieldlimit = fldl;
		cloud = cl;
		mapdesc = md;
		mapname = mn;
		mapmark = mm;
		bgm = bm;
		swim = s;
		town = tw;
		hideminimap = hm;
		mapwalls = wll;
		mapborders = bnd;
		playerpos = vector2d();
	}

	void maplemap::draw(ID2D1HwndRenderTarget* target, vector2d position)
	{
		for (vector<background>::iterator back = backgrounds.begin(); back != backgrounds.end(); back++)
		{
			back->draw(target, position);
		}
		for (char i = 0; i < 7; i++)
		{
			for (map<char, vector<sprite>>::iterator obj = objlayers[i].begin(); obj != objlayers[i].end(); obj++)
			{
				for (vector<sprite>::iterator tlit = obj->second.begin(); tlit != obj->second.end(); tlit++)
				{
					tlit->draw(target, position);
				}
			}

			for (map<char, vector<sprite>>::iterator tile = tilelayers[i].begin(); tile != tilelayers[i].end(); tile++)
			{
				for (vector<sprite>::iterator tlit = tile->second.begin(); tlit != tile->second.end(); tlit++)
				{
					tlit->draw(target, position);
				}
			}
		}
		for (map<int, npc>::iterator npc = npcs.begin(); npc != npcs.end(); npc++)
		{
			npc->second.draw(target, position);
		}
		for (map<int, mob>::iterator mob = mobs.begin(); mob != mobs.end(); mob++)
		{
			mob->second.draw(target, position);
		}

		//playerchar->draw(target, position);

		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			pit->second.draw(target, position);
		}
		for (vector<background>::iterator front = foregrounds.begin(); front != foregrounds.end(); front++)
		{
			front->draw(target, position);
		}
	}

	void maplemap::update(vector2d plpos)
	{
		playerpos = plpos;

		for (vector<background>::iterator back = backgrounds.begin(); back != backgrounds.end(); back++)
		{
			back->update();
		}
		for (char i = 0; i < 7; i++)
		{
			for (map<char, vector<sprite>>::iterator obj = objlayers[i].begin(); obj != objlayers[i].end(); obj++)
			{
				for (vector<sprite>::iterator objit = obj->second.begin(); objit != obj->second.end(); ++objit)
				{
					objit->update();
				}
			}
		}

		for (map<int, npc>::iterator npc = npcs.begin(); npc != npcs.end(); npc++)
		{
			npc->second.update();
		}
		for (map<int, mob>::iterator mob = mobs.begin(); mob != mobs.end(); mob++)
		{
			mob->second.update();
		}
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			if (colliding(make_pair(playerpos, vector2d(50, 80)), make_pair(pit->second.getposition(), pit->second.getdimension())))
			{
				switch (pit->second.gettype())
				{
				case PT_TOUCH:
					//game.enterportal(pit->second.getwarpinfo());
					break;
				case PT_HIDDEN:
				case PT_SCRIPTED_HIDDEN:
					pit->second.settouch(true);
					break;
				}
			}
			pit->second.update();
		}
		for (vector<background>::iterator front = foregrounds.begin(); front != foregrounds.end(); front++)
		{
			front->update();
		}
	}

	void maplemap::addreactor(int oid, reactor toadd)
	{
		/*reactor toadd = game.getprovider->loadreactor(id);
		toadd.setinfo(oid, state, pos);
		reactors[oid] = toadd;*/
	}

	void maplemap::addnpc(int oid, npc toadd)
	{
		npcs.insert(make_pair(oid, toadd));
	}

	void maplemap::addmob(int oid, mob toadd)
	{
		mobs.insert(make_pair(oid, toadd));
	}

	int maplemap::getid()
	{
		return mapid;
	}

	vector2d maplemap::getspawnp(char id)
	{
		return portals[id].getposition();
	}

	vector2d maplemap::getspawnp(string pname)
	{
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			if (pit->second.getname() == pname)
				return pit->second.getposition();
		}
		return playerpos;
	}

	pair<int, string> maplemap::getportal()
	{
		portal* front = 0;
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			if (colliding(make_pair(playerpos, vector2d(50, 80)), make_pair(pit->second.getposition(), pit->second.getdimension())) && pit->second.gettype() != PT_WARP)
				front = &pit->second;
		}

		if (front)
			return front->getwarpinfo();
		else
			return make_pair(-1, "");
	}

	fthold maplemap::getgroundbelow(vector2d pos)
	{
		fthold ret = footholds.begin()->second;
		int ycomp = 65536;
		pos = pos - vector2d(0, 5);
		for (map<short, fthold>::iterator ftit = footholds.begin(); ftit != footholds.end(); ftit++)
		{
			fthold cur = ftit->second;
			if (cur.horizontal.contains(pos.x()) && ycomp > cur.vertical.y() && cur.vertical.y() > pos.y() && cur.vertical.straight())
			{
				ret = cur;
				ycomp = cur.vertical.y();
			}
		}
		return ret;
	}

	fthold maplemap::getnextground(fthold last, vector2d pos, bool left)
	{
		short id = (left) ? last.prev : last.next;
		if (id > 0)
		{
			return footholds[id];
		}
		else
		{
			return getgroundbelow(pos);
		}
	}

	fthold maplemap::getleftbound(fthold last)
	{
		return last;
	}

	fthold maplemap::getrightbound(fthold last)
	{
		return last;
	}
}