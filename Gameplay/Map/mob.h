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

using namespace std;
using namespace util;
using namespace graphics;

namespace gameplay
{
	class mob
	{
	private:
		map<string, animation> textures;
		int oid;
		string name;
		short level;
		short speed;
		vector2d pos;
		vector2d walls;
		string state;
		short moved;
		char stance;
		short fh;
		char effect;
		bool fadein;
		char team;
	public:
		mob() {}
		~mob() {}
		mob(map<string, animation>, string, short, short);
		void setinfo(int, vector2d, vector2d, char, short, char, bool, char);
		void update();
		void draw(ID2D1HwndRenderTarget*, vector2d);
	};
}

