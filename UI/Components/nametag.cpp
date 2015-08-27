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
#include "nametag.h"

namespace io
{
	nametag::nametag(IDWriteTextFormat* fnt, textcolor col, pair<vector<texture>, vector<texture>> t, string n, vector2d p, bool a)
	{
		content = textlabel(fnt, col, n);
		tag = t;
		position = p;
		active = a;
	}

	nametag::nametag(IDWriteTextFormat* fnt, textcolor col, vector<texture> t, string n, vector2d p)
	{
		content = textlabel(fnt, col, n);
		tag.first = t;
		position = p;
		active = false;
	}

	void nametag::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;
		vector2d bgpos = absp - vector2d(-52, -2);

		if (!active)
		{
			tag.first[0].draw(bgpos);
			char i;
			for (i = 0; i < content.gettext().length() / 3; i++)
			{
				tag.first[1].draw(bgpos + vector2d(8 + 9 * i, 0));
			}
			tag.first[2].draw(bgpos + vector2d(8 + 9 * i, 0));
		}
		else
		{
			tag.second[0].draw(bgpos);
			char i;
			for (i = 0; i < content.gettext().length() / 3; i++)
			{
				tag.second[1].draw(bgpos + vector2d(8 + 9 * i, 0));
			}
			tag.second[2].draw(bgpos + vector2d(8 + 9 * i, 0));
		}

		content.draw(target, absp + vector2d(-13, 0));
	}
}
