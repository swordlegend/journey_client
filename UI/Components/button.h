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
#include "texture.h"

using namespace util;
using namespace std;
using namespace graphics;

namespace io
{
	enum buttontypes : char
	{
		BTT_REGULAR,
		BTT_ONESPRITE,
		BTT_AREA
	};

	class button
	{
	private:
		map<string, texture> sprites;
		vector2d position;
		vector2d dimension;
		string state;
		char bttype;
	public:
		button(node, int, int);
		button(texture, texture, int, int);
		button(int, int, int, int);
		button() {}
		~button() {}
		pair<vector2d, vector2d> bounds();
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void setstate(string);
		string getstate();
	};
}

