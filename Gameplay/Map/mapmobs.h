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
#include "mob.h"
#include "safemap.h"

using namespace util;

namespace gameplay
{
	class mapmobs
	{
	public:
		mapmobs() {}
		~mapmobs() {}
		void addmob(int, int, bool, vector2d, char, short, char, bool, char);
		void sendattack(attackinfo*, int, pair<vector2d, vector2d>);
		void sendmobhp(int, char);
		void movemob(int, char, bool);
		void killmob(int, char);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
		void clear();
	private:
		safemap<int, mob> mobs;
	};
}

