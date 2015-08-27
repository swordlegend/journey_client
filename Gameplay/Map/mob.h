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
#include "animation.h"
#include "attackinfo.h"
#include "charset.h"

using namespace std;
using namespace util;
using namespace graphics;

namespace gameplay
{
	class mob
	{
	public:
		mob() {}
		~mob() {}
		mob(int, int, bool, vector2d, char, short, char, bool, char);
		bool update();
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void damage(attackinfo*, int);
		void showhp(char);
		void setmove(char, bool);
		void setstate(string st) { state = st; }
		void kill(char);
		bool isalive() { return state != "die1"; }
		vector2d getposition() { return pos; }
		vector2d getdimension() { return textures[state].getdimension(0); }
	private:
		map<string, animation> textures;
		map<string, texture> uitextures;
		float alpha;
		int oid;
		string name;
		short level;
		short knockback;
		char hppercent;
		short speed;
		vector2d pos;
		vector2d absp;
		vector2d walls;
		string state;
		bool control;
		float moved;
		char stance;
		short fh;
		char effect;
		bool fadein;
		bool dead;
		bool fade;
		bool fleft;
		char team;
		float hspeed;
		float vspeed;
		float fx;
		float fy;
	};
}

