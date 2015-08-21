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
#include "Journey.h"

namespace graphics
{
	texture::texture()
	{
		origin = vector2d();
		dimension = vector2d();
	}

	texture::texture(node src)
	{
		bitmap bmp = src.get_bitmap();
		source = app.getimgcache()->createimage(bmp);
		dimension = vector2d(bmp.width(), bmp.height());
		node originnode = src["origin"];
		if (originnode.data_type() == nl::node::type::vector)
			origin = vector2d(originnode.x(), originnode.y());
		else
			origin = vector2d();
	}

	void texture::draw(vector2d position)
	{
		draw(position, 1.0f);
	}

	void texture::draw(vector2d position, float alpha)
	{
		draw(position, vector2d(), alpha);
	}

	void texture::draw(vector2d position, vector2d stretch)
	{
		draw(position, stretch, 1.0f);
	}

	void texture::draw(vector2d position, vector2d stretch, float alpha)
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

			app.getimgcache()->draw(source.first, source.second, r, alpha);
		}
	}

	void texture::shift(vector2d shf)
	{
		origin = origin - shf;
	}
}
