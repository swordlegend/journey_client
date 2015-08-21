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
#include "bitmap.h"

using namespace std;
using namespace nl;

namespace program
{
	enum imgcontext : char
	{
		ict_login,
		ict_sys,
		ict_map
	};

	class imagecache
	{
	public:
		imagecache() {}
		~imagecache() {}
		void init(IWICImagingFactory*);
		void setmode(imgcontext);
		void settarget(ID2D1HwndRenderTarget* trg) { target = trg; }
		void unlock();
		void clearcache(imgcontext);
		void draw(imgcontext, size_t, D2D1_RECT_F, float);
		pair<imgcontext, size_t> createimage(bitmap bmp);
	private:
		unique_ptr<IWICImagingFactory> imgfactory;
		ID2D1HwndRenderTarget* target;
		map<imgcontext, map<size_t, IWICBitmap*>> temp;
		map<imgcontext, map<size_t, ID2D1Bitmap*>> cache;
		imgcontext imgcon;
		mutex modelock;
	};
}

