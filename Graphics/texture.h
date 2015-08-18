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
#include "vector2d.h"

using namespace util;
using namespace std;

namespace graphics
{
	class texture
	{
	public:
		texture();
		texture(pair<data::cachemode, size_t>, vector2d, vector2d);
		~texture();
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void draw(ID2D1HwndRenderTarget*, vector2d, float);
		void draw(ID2D1HwndRenderTarget*, vector2d, vector2d);
		void draw(ID2D1HwndRenderTarget*, vector2d, vector2d, float);
		void shift(vector2d);
		vector2d getdimension() { return dimension; }
		vector2d getorigin() { return origin; }
	private:
		ID2D1Bitmap* bitmap;
		pair<data::cachemode, size_t> source;
		vector2d origin;
		vector2d dimension;
	};
}

