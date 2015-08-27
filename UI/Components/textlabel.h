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
	enum textcolor
	{
		txc_black,
		txc_white,
		txc_yellow,
		txc_blue,
		txc_red
	};

	class textlabel
	{
	public:
		textlabel(IDWriteTextFormat*, textcolor, string);
		textlabel() {}
		~textlabel() {}
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void settext(string s) { text = s; }
		void setmarker(bool b) { marker = b; }
		void setalpha(float a) { alpha = a; }
		string gettext() { return text; }
	private:
		IDWriteTextFormat* font;
		ID2D1SolidColorBrush* brush;
		textcolor color;
		string text;
		float alpha;
		bool marker;
		vector2d position;
	};
}

