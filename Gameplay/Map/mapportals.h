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
#include "collision.h"
#include "portal.h"
#include "node.h"

using namespace util;

namespace gameplay
{
	class mapportals
	{
	public:
		mapportals() {}
		~mapportals() {}
		void clear();
		void addportal(char, portal);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update(vector2d);
		vector2d getspawnpoint(char);
		vector2d getspawnpoint(string);
		pair<int, string> getportal(vector2d);
	private:
		map<char, portal> portals;
	};
}

