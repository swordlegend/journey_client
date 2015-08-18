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
#include "nxprovider.h"
#include "nxfile.h"

using namespace std;
using namespace graphics;

namespace io
{
	enum mousestate : char {
		MST_IDLE = 0,
		MST_CANCLICK = 1,
		MST_GAME = 2,
		MST_HOUSE = 3,
		MST_CANCLICK2 = 4,
		MST_CANGRAB = 5,
		MST_GIFT = 6,
		MST_VSCROLL = 7,
		MST_HSCROLL = 8,
		MST_VSCROLLIDLE = 9,
		MST_HSCROLLIDLE = 10,
		MST_GRABBING = 11,
		MST_CLICKING = 12,
		MST_RCLICK = 13
	};

	class cursor
	{
	private:
		mousestate state;
		map<mousestate, animation> sprites;
		vector2d position;
	public:
		cursor();
		~cursor();
		void init(nxprovider*);
		void update(vector2d);
		void update();
		void draw(ID2D1HwndRenderTarget*);
		char getstate();
		vector2d getposition();
		void setstate(char);
	};
}

