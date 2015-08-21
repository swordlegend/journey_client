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
#include "playfield.h"
#include "Journey.h"

namespace gameplay
{
	bool playfield::moveup()
	{
		pair<int, string> warpinfo = portals.getportal(playerchar.getposition());
		if (warpinfo.first == map_info.getid())
		{
			vector2d spawnpos = portals.getspawnpoint(warpinfo.second);
			playerchar.setposition(spawnpos);
		}
		else if (warpinfo.first != 0)
		{
			packet_c.changemap(false, warpinfo.first, warpinfo.second, false);
			return false;
		}
		return true;
	}

	void playfield::useattack(int skillid)
	{
		if (playerchar.attack(skillid))
		{
			if (skillid == -1)
			{
				bool left = playerchar.getleft();

				pair<vector2d, vector2d> attackrange;
				attackrange.first = playerchar.getposition();
				if (left)
				{
					attackrange.second = vector2d(-100, 80);
				}
				else
				{
					attackrange.second = vector2d(100, 80);
				}

				int basedamage = playerchar.getstats()->basedamage;

				attackinfo attack;
				attack.mastery = 0.5f; //todo: get from char
				attack.skill = 0;
				attack.maxattacked = 1;
				attack.display = 5;
				attack.charge = 0;
				attack.speed = 2; //todo: wep speed of char
				attack.direction = (left) ? 1 : 0;
				attack.numdamage = 1;
				attack.stance = 1; //maybe body animation?

				map_objects.sendattack(&attack, basedamage, attackrange);
				packet_c.close_attack(attack);
			}
			else
			{
				//get skill
			}
		}
	}

	void playfield::draw(ID2D1HwndRenderTarget* target)
	{
		if (step == GST_GAME)
		{
			view.update(playerchar.getposition());

			backgrounds.drawbackgrounds(target, view.getposition());
			maplayers[0].draw(target, view.getposition());
			maplayers[1].draw(target, view.getposition());
			maplayers[2].draw(target, view.getposition());
			maplayers[3].draw(target, view.getposition());
			maplayers[4].draw(target, view.getposition());
			maplayers[5].draw(target, view.getposition());
			maplayers[6].draw(target, view.getposition());
			maplayers[7].draw(target, view.getposition());
			map_objects.draw(target, view.getposition());
			playerchar.draw(target, view.getposition());
			portals.draw(target, view.getposition());
			backgrounds.drawforegrounds(target, view.getposition());
		}
	}

	void playfield::update()
	{
		if (step == GST_GAME)
		{
			backgrounds.update();
			map_objects.update();
			playerchar.update();
			portals.update(playerchar.getposition());

			for (int i = 0; i < 8; i++)
			{
				maplayers[i].update();
			}
		}
	}

	void playfield::setplayer(player plchar)
	{
		step = GST_TRANSITION;

		m_account.clear();
		playerchar = plchar;
	}

	void playfield::setfield(int mapid, char pid)
	{
		step = GST_TRANSITION;

		map_objects.clear();
		portals.clear();

		app.getimgcache()->setmode(ict_map);
		nl::nx::view_file("Map");

		string fullname;
		string strid = to_string(mapid);
		char extend = 9 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		node mapdata = nl::nx::nodes["Map"].resolve("Map/Map" + to_string(mapid / 100000000) + "/" + fullname.append(strid) + ".img");

		map_info = mapinfo(mapid, mapdata);
		footholds = footholdtree(mapdata.resolve("foothold"));

		nl::node portalnodes = mapdata.resolve("portal");
		for (nl::node portalnd = portalnodes.begin(); portalnd != portalnodes.end(); portalnd++)
		{
			char pid = static_cast<char>(stoi(portalnd.name()));

			portaltype ptype = static_cast<portaltype>(portalnd.resolve("pt").get_integer());
			string pname = portalnd.resolve("pn").get_string();
			vector2d ppos = vector2d(static_cast<int>(portalnd.resolve("x").get_integer()), static_cast<int>(portalnd.resolve("y").get_integer()));
			int targetid = static_cast<int>(portalnd.resolve("tm").get_integer());
			string targetpname = portalnd.resolve("tn").get_string();

			animation anim;

			nl::node pnode = nl::nx::nodes["Map"].resolve("MapHelper.img/portal/game");
			switch (ptype)
			{
			case PT_REGULAR:
				anim = animation(pnode.resolve("pv"));
				break;
			case PT_HIDDEN:
			case PT_SCRIPTED_HIDDEN:
				anim = animation(pnode.resolve("ph/default/portalContinue"));
				break;
			}

			portals.addportal(pid, portal(ptype, pname, targetid, targetid == mapid, targetpname, anim, ppos));
		}

		nl::nx::view_file("Back");

		backgrounds = mapbackgrounds(mapdata.resolve("back"), map_info.getwalls(), map_info.getborders());

		nl::nx::unview_file("Back");

		nl::nx::view_file("Tile");
		nl::nx::view_file("Obj");

		for (int i = 0; i < 8; i++)
		{
			maplayers[i] = maplayer(mapdata.resolve(to_string(i)));
		}

		nl::nx::unview_file("Tile");
		nl::nx::unview_file("Obj");
		nl::nx::unview_file("Map");
		app.getimgcache()->unlock();

		vector2d startpos = portals.getspawnpoint(pid);
		playerchar.setfh(&footholds);
		playerchar.setposition(startpos);
		view.setposition(startpos);
		view.setbounds(map_info.getwalls(), map_info.getborders());

		step = GST_GAME;
	}
}
