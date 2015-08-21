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
	nametag::nametag() {}
	nametag::~nametag() {}

	nametag::nametag(IDWriteTextFormat* fnt, textcolor col, pair<vector<texture>, vector<texture>> t, string n, vector2d p, bool a)
	{
		font = fnt;
		color = col;
		tag = t;
		name = n;
		position = p;
		active = a;
	}

	void nametag::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;
		vector2d bgpos = absp - vector2d(-52, -2);

		if (!active)
		{
			tag.first[0].draw(bgpos);
			char i;
			for (i = 0; i < name.length() / 3; i++)
			{
				tag.first[1].draw(bgpos + vector2d(8 + 9 * i, 0));
			}
			tag.first[2].draw(bgpos + vector2d(8 + 9 * i, 0));
		}
		else
		{
			tag.second[0].draw(bgpos);
			char i;
			for (i = 0; i < name.length() / 3; i++)
			{
				tag.second[1].draw(bgpos + vector2d(8 + 9 * i, 0));
			}
			tag.second[2].draw(bgpos + vector2d(8 + 9 * i, 0));
		}

		D2D1_RECT_F layoutRect = D2D1::RectF(
			static_cast<FLOAT>(absp.x()),
			static_cast<FLOAT>(absp.y()),
			static_cast<FLOAT>(absp.x() + (12 * name.length())),
			static_cast<FLOAT>(absp.y() + 24)
			);

		std::wstring wide_string(name.begin(), name.end());

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

		brush->Release();
	}
}
