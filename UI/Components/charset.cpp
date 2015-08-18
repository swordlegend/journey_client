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
#include "charset.h"
#include "nxprovider.h"

namespace io
{
	charset::charset(map<char, texture> ch)
	{
		characters = ch;
	}

	int charset::draw(ID2D1HwndRenderTarget* target,char c, vector2d pos)
	{
		characters[c].draw(target, pos);
		return getw(c);
	}

	int charset::getw(char c)
	{
		return characters[c].getdimension().x();
	}

	int charset::draw(ID2D1HwndRenderTarget* target, string numstr, alignment align, vector2d pos)
	{
		char length = numstr.length();
		int shift = 0;

		switch (align)
		{
		case cha_left:
			for (char i = 0; i < length; i++)
			{
				shift += getw(numstr[i]);
				draw(target, numstr[i], pos + vector2d(shift, 0));
			}
			break;
		case cha_right:
			for (char i = length - 1; i >= 0; i--)
			{
				shift += getw(numstr[i]);
				draw(target, numstr[i], pos - vector2d(shift, 0));
			}
			break;
		}
		return shift;
	}

	int charset::draw(ID2D1HwndRenderTarget* target, string numstr, char space, alignment align, vector2d pos)
	{
		char length = numstr.length();
		int shift = 0;

		switch (align)
		{
		case cha_center:
			shift -= (space * length) / 2;
		case cha_left:
			for (char i = 0; i < length; i++)
			{
				shift += space;
				draw(target, numstr[i], pos + vector2d(shift, 0));
			}
			break;
		case cha_right:
			for (char i = length - 1; i >= 0; i--)
			{
				shift += space;
				draw(target, numstr[i], pos - vector2d(shift, 0));
			}
			break;
		}
		return shift;
	}
}
