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

using namespace graphics;

namespace gameplay
{
	class itemdrop
	{
	public:
		itemdrop(texture, vector2d, vector2d, int, int, int, bool, char, bool);
		itemdrop() {}
		~itemdrop() {}
		void draw(ID2D1HwndRenderTarget*, vector2d);
		bool update();
		void expire() { pickedup = true; }
	private:
		texture icon;
		vector2d position;
		vector2d destination;
		vector2d borders;
		int oid;
		int itemid;
		int owner;
		bool meso;
		char pickuptype;
		float alpha;
		bool playerdrop;
		bool pickedup;
		bool floating;
		float vspeed;
		float fy;
	};
}

