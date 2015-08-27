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
#include "textlabel.h"

using namespace graphics;

namespace io
{
	class textfield
	{
	public:
		textfield(IDWriteTextFormat*, textcolor, string, vector2d, int);
		textfield() {}
		~textfield() {}
		pair<vector2d, vector2d> bounds();
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
		void setstate(string);
		void sendchar(char);
		void setbg(texture, int, int);
		string getstate() { return state; }
		string text() { return content.gettext(); }
	private:
		texture bg;
		textlabel content;
		vector2d position;
		string state;
		vector2d bgposition;
		int maxlength;
		int markpos;
		bool showmark;
		short elapsed;
	};
}

