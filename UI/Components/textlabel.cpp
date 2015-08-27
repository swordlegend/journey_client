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
#include "textlabel.h"

namespace graphics
{
	textlabel::textlabel(IDWriteTextFormat* f, textcolor c, string s)
	{
		font = f;
		color = c;
		text = s;
		marker = false;
		position = vector2d(0, 5);
		alpha = 1.0f;
		brush = 0;
	}

	void textlabel::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;

		string temp = text;

		if (marker)
		{
			temp.append("|");
		}

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(absp.x()),
			static_cast<FLOAT>(absp.y()),
			static_cast<FLOAT>(absp.x() + (font->GetFontSize() * temp.length())),
			static_cast<FLOAT>(absp.y() + font->GetFontSize())
			);

		wstring wide_string(temp.begin(), temp.end());

		if (!brush)
		{
			switch (color)
			{
			case txc_white:
				target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
				break;
			case txc_black:
				target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
				break;
			case txc_yellow:
				target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);
				break;
			}
		}

		brush->SetOpacity(alpha);

		target->DrawText(
			wide_string.c_str(),
			wide_string.length(),
			font,
			layoutRect,
			brush
			);
	}
}
