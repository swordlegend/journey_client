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
#include "texture.h"
#include "nxprovider.h"

namespace graphics
{
	texture::texture()
	{
		origin = vector2d();
		dimension = vector2d();
		bitmap = 0;
	}

	texture::texture(pair<cachemode, size_t> src, vector2d org, vector2d dim)
	{
		source = src;
		origin = org;
		dimension = dim;
		bitmap = 0;
	}

	texture::~texture()
	{
		if (bitmap)
		{
			bitmap->Release();
		}
	}

	void texture::draw(ID2D1HwndRenderTarget* target, vector2d position)
	{
		draw(target, position, 1.0f);
	}

	void texture::draw(ID2D1HwndRenderTarget* target, vector2d position, float alpha)
	{
		draw(target, position, vector2d(), alpha);
	}

	void texture::draw(ID2D1HwndRenderTarget* target, vector2d position, vector2d stretch)
	{
		draw(target, position, stretch, 1.0f);
	}

	void texture::draw(ID2D1HwndRenderTarget* target, vector2d position, vector2d stretch, float alpha)
	{
		if (!bitmap)
		{
			target->CreateBitmapFromWicBitmap(data::getbitmap(source.first, source.second), &bitmap);
		}

		if (bitmap)
		{
			int w = stretch.x();
			if (w == 0)
				w = dimension.x();

			int h = stretch.y();
			if (h == 0)
				h = dimension.y();

			vector2d absp = position - origin;

			if (absp.x() <= 816 && absp.y() <= 624 && absp.x() > -dimension.x() && absp.y() > -dimension.y())
			{
				D2D1_RECT_F r = { static_cast<float>(absp.x()),
					static_cast<float>(absp.y()),
					static_cast<float>(absp.x() + w),
					static_cast<float>(absp.y() + h) };

				target->DrawBitmap(bitmap, r, alpha);
			}
		}
	}

	void texture::shift(vector2d shf)
	{
		origin = origin - shf;
	}
}
