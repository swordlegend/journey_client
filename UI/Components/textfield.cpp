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
		content = textlabel(fnt, col, default);
		position = pos;
		maxlength = length;
		state = "inactive";
		showmark = false;
		markpos = default.length();
	}

	pair<vector2d, vector2d> textfield::bounds()
	{
		return pair<vector2d, vector2d>(position, vector2d(12 * maxlength, 24));
	}

	void textfield::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;

		if (content.gettext() == "" && state == "inactive")
		{
			bg.draw(absp + bgposition);
		}
		else
		{
			if (state == "active" && showmark && markpos == content.gettext().length())
			{
				content.setmarker(true);
			}
			else
			{
				content.setmarker(false);
			}

			content.draw(target, absp);
		}
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
		markpos = content.gettext().length();
	}

	void textfield::sendchar(char letter)
	{
		if (letter == 0 && markpos > 0)
		{
			markpos--;
			content.settext(content.gettext().erase(markpos, 1));
		}
		else if (letter > 2 && content.gettext().length() < maxlength)
		{
			content.settext(content.gettext().insert(markpos, &letter));
			markpos++;
		}
		else if (letter == 1 && markpos > 0)
		{
			markpos -= 1;
		}
		else if (letter == 2 && markpos < content.gettext().length())
		{
			markpos += 1;
		}
	}
}
