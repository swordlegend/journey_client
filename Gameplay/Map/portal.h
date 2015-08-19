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
	enum portaltype : char
	{
		PT_SPAWN,
		PT_INVISIBLE,
		PT_REGULAR,
		PT_TOUCH,
		PT_4,
		PT_5,
		PT_WARP,
		PT_SCRIPTED,
		PT_SCRIPTED_INVISIBLE,
		PT_SCRIPTED_TOUCH,
		PT_HIDDEN,
		PT_SCRIPTED_HIDDEN,
		PT_SPRING1,
		PT_SPRING2,
		PT_14
	};

	class portal
	{
	private:
		animation anim;
		portaltype type;
		string pname;
		vector2d position;
		int targetid;
		string targetpname;
		bool touched;
		bool intermap;
	public:
		portal();
		~portal();
		portal(portaltype, string, int, bool, string, animation, vector2d);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
		void settouch(bool);
		vector2d getposition();
		vector2d getdimension();
		portaltype gettype();
		pair<int, string> getwarpinfo();
		string getname();
	};
}

