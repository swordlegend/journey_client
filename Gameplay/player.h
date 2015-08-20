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
#include "maplechar.h"
#include "inventory.h"
#include "footholdtree.h"

using namespace std;

namespace gameplay
{
	const float fallspeed = 7.7f;
	const float gravityAcc = 0.5f;
	const float jumpSpeed = 9.0f;
	const float walkSpeed = 2.25f;

	enum movecode : short
	{
		MC_NONE = 0x0000,
		MC_LEFT = 0x0001,
		MC_RIGHT = 0x0010,
		MC_JUMP = 0x0100,
		MC_CROUCH = 0x1000
	};

	struct skill
	{
		char level;
		char masterlevel;
		long expiration;
	};

	struct startedquest
	{
		string questdata;
		short infonumber;
		string medaldata;
	};

	class player
	{
	private:
		maplestats stats;
		maplelook look;
		inventory invent;
		map<int, pair<pair<int, int>, long>> skills;
		map<int, short> cooldowns;
		map<int, pair<string, pair<short, string>>> quests;
		map<int, long> completedquests;
		pair<vector<int>, vector<int>> trockmaps;
		int bookcover;
		map<short, char> bookcards;
		map<short, string> areainfo;
		vector2d position;
		short movestate;
		footholdtree* footholds;
		short speed;
		short jump;
		float hspeed;
		float vspeed;
		float fx;
		float fy;
		float ground;
		bool fleft;
		bool nofriction;
		void recalcstats(bool);
		bool walking;
		bool standing;
	public:
		player();
		~player();
		player(maplechar*, inventory, map<int, pair<pair<int, int>, long>>, map<int, short>, map<int, pair<string, pair<short, string>>>, map<int, long>, pair<vector<int>, vector<int>>, int, map<short, char>, map<short, string>);
		maplestats* getstats();
		inventory* getinventory();
		vector2d getposition();
		bool getleft() { return fleft; }
		void setposition(vector2d);
		void setfh(footholdtree*);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
		void crouch(bool);
		void sit(bool);
		void move(movecode, bool);
		void setexpression(char);
		void updateskill(int, int, int, long);
	};
}

