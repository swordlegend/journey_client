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
#include "node.h"
#include "vector2d.h"
#include "imagecache.h"

using namespace util;
using namespace nl;
using namespace std;
using namespace program;

namespace graphics
{
	class texture
	{
	public:
		texture(node);
		texture();
		~texture() {}
		void draw(vector2d);
		void draw(vector2d, float);
		void draw(vector2d, vector2d);
		void draw(vector2d, vector2d, float);
		void shift(vector2d);
		vector2d getdimension() { return dimension; }
		vector2d getorigin() { return origin; }
	private:
		pair<imgcontext, size_t> source;
		vector2d origin;
		vector2d dimension;
	};
}

