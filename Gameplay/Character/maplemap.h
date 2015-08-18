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
#include "stdfax.h"
#include "sprite.h"
#include "background.h"
#include "npc.h"
#include "mob.h"
#include "reactor.h"
#include "portal.h"

using namespace io;
using namespace std;
using namespace data;

namespace gameplay
{
	enum mapobjecttype : char
	{
		MO_CHARACTER,
		MO_NPC,
		MO_MOB,
		MO_REACTOR,
		MO_ITEMDROP
	};

	struct foothold
	{
		short next;
		short prev;
		vector2d horizontal;
		vector2d vertical;
	};

	typedef struct foothold fthold;

	struct mapinfo
	{
		int mapid;
		int fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		vector2d mapwalls;
		vector2d mapborders;
	};

	struct mapsprites
	{
		map<char, map<char, vector<sprite>>> tilelayers;
		map<char, map<char, vector<sprite>>> objlayers;
		vector<background> backgrounds;
		vector<background> foregrounds;
	};

	struct mapobjects
	{
		map<int, npc> npcs;
		map<int, mob> mobs;
		map<int, reactor> reactors;
		map<char, portal> portals;

		void addreactor(int, reactor);
		void addnpc(int, npc);
		void addmob(int, mob);
		vector2d getspawnpoint(string);
		pair<int, string> getportal(vector2d);
	};

	class maplemap
	{
	private:
		map<char, map<char, vector<sprite>>> tilelayers;
		map<char, map<char, vector<sprite>>> objlayers;
		vector<background> backgrounds;
		vector<background> foregrounds;
		map<int, mapobjecttype> mapobjects;
		map<int, npc> npcs;
		map<int, mob> mobs;
		map<int, reactor> reactors;
		map<char, portal> portals;
		map<short, fthold> footholds;
		int mapid;
		int fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		vector2d playerpos;
		vector2d mapwalls;
		vector2d mapborders;
	public:
		maplemap() {}
		~maplemap() {}
		maplemap(int, map<char, map<char, vector<sprite>>>, map<char, map<char, vector<sprite>>>, vector<background>, vector<background>, map<char, portal>, map<short, struct foothold>, int, bool, string, string, string, string, string, bool, bool, bool, vector2d, vector2d);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update(vector2d);
		void addreactor(int, reactor);
		void addnpc(int, npc);
		void addmob(int, mob);
		int getid();
		vector2d getspawnp(char);
		vector2d getspawnp(string);
		pair<int, string> getportal();
		fthold getgroundbelow(vector2d);
		fthold getnextground(fthold, vector2d, bool);
		fthold getleftbound(fthold);
		fthold getrightbound(fthold);
		vector2d getwalls() { return mapwalls; }
		vector2d getborders() { return mapborders; }
	};
}