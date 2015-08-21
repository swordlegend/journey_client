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
#include "textfield.h"

namespace io
{
	textfield::textfield(IDWriteTextFormat* fnt, textcolor col, string default, vector2d pos, int length)
	{
		font = fnt;
		color = col;
		content = default;
		position = pos;
		maxlength = length;
		state = "inactive";
		showmark = false;
		markpos = content.length();
	}

	pair<vector2d, vector2d> textfield::bounds()
	{
		return pair<vector2d, vector2d>(position, vector2d(12 * maxlength, 24));
	}

	void textfield::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;

		if (content == "" && state == "inactive")
		{
			bg.draw(absp + bgposition);
			return;
		}

		string temp = content;

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(absp.x()),
			static_cast<FLOAT>(absp.y()),
			static_cast<FLOAT>(absp.x() + (12 * maxlength)),
			static_cast<FLOAT>(absp.y() + 24)
			);

		if (state == "active" && showmark && markpos == content.length())
		{
			temp.append("|");
		}

		wstring wide_string(temp.begin(), temp.end());

		ID2D1SolidColorBrush* brush = 0;
		switch (color)
		{
		case TXC_WHITE:
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
			break;
		case TXC_BLACK:
			target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
			break;
		}

		target->DrawText(
			wide_string.c_str(),
			wide_string.length(),
			font,
			layoutRect,
			brush
			);
	}

	void textfield::update()
	{
		elapsed += 16;
		if (elapsed >= 300)
		{
			showmark = !showmark;
			elapsed -= 300;
		}
	}

	void textfield::setbg(texture b, int x, int y)
	{
		bg = b;
		bgposition = vector2d(x, y);
	}

	void textfield::setstate(string st)
	{
		elapsed = 0;
		showmark = true;
		state = st;
		markpos = content.length();
	}

	void textfield::sendchar(char letter)
	{
		if (letter == 0 && markpos > 0)
		{
			markpos--;
			content.erase(markpos, 1);
		}
		else if (letter > 2 && content.length() < maxlength)
		{
			content.insert(markpos, &letter);
			markpos++;
		}
		else if (letter == 1 && markpos > 0)
		{
			markpos -= 1;
		}
		else if (letter == 2 && markpos < content.length())
		{
			markpos += 1;
		}
	}

	string textfield::getstate()
	{
		return state;
	}

	string textfield::text()
	{
		return content;
	}
}
