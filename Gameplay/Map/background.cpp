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
#include "background.h"

namespace gameplay
{
	background::background(node src, node back, vector2d walls, vector2d borders)
	{
		ani = animation(back[src["bS"].get_string() + ".img"][(src["ani"].get_bool()) ? "ani" : "back"][to_string(src["no"].get_integer())]);
		type = static_cast<bgtype>(src["type"].get_integer());
		flipped = src["f"].get_bool();
		pos = vector2d(static_cast<int>(src["x"]), static_cast<int>(src["y"]));
		rpos = vector2d(static_cast<int>(src["rx"]), static_cast<int>(src["ry"]));
		cpos = vector2d(static_cast<int>(src["cx"]), static_cast<int>(src["cy"]));
		alpha = static_cast<float>(src["a"].get_integer() / 255);

		mapwalls = walls;
		mapborders = borders;

		if (cpos.x() == 0)
			cpos = vector2d(ani.getdimension(0).x(), cpos.y());
		if (cpos.y() == 0)
			cpos = vector2d(cpos.x(), ani.getdimension(0).y());

		fx = static_cast<float>(pos.x());
		fy = static_cast<float>(pos.y());

		switch (type)
		{
		case BGT_HMOVEA:
		case BGT_HMOVEB:
			hspeed = 0.5f;
			break;
		case BGT_VMOVEA:
		case BGT_VMOVEB:
			vspeed = 0.5f;
			break;
		default:
			hspeed = 0;
			vspeed = 0;
		}
	}

	void background::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		pos = vector2d(static_cast<int>(fx), static_cast<int>(fy));
		vector2d absp = pos + parentpos;

		if (flipped)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(-1.0f, 1.0f),
				D2D1::Point2F(
				(float)absp.x(),
				(float)absp.y())));
		}

		switch (type)
		{
		case BGT_HMOVEA:
		case BGT_HTILED:
			for (short i = pos.x(); i > mapwalls.x() - cpos.x(); i = i - cpos.x())
			{
				pos = vector2d(i, pos.y());
			}
			for (short i = pos.x(); i < mapwalls.y() + cpos.x(); i = i + cpos.x())
			{
				ani.draw(target, vector2d(i + parentpos.x(), absp.y()), alpha);
			}
			break;
		case BGT_VMOVEA:
		case BGT_VTILED:
			for (short i = pos.y(); i > mapborders.x() - cpos.y(); i = i - cpos.y())
			{
				pos = vector2d(pos.x(), i);
			}
			for (short i = pos.y(); i < mapborders.y() + cpos.y(); i = i + cpos.y())
			{
				ani.draw(target, vector2d(absp.x(), i + parentpos.y()), alpha);
			}
			break;
		case BGT_HMOVEB:
		case BGT_VMOVEB:
		case BGT_TILED:
			for (short i = pos.x(); i > mapwalls.x() - cpos.x(); i = i - cpos.x())
			{
				pos = vector2d(i, pos.y());
			}
			for (short i = pos.y(); i > mapborders.x() - cpos.y(); i = i - cpos.y())
			{
				pos = vector2d(pos.x(), i);
			}
			for (short i = pos.x(); i < mapwalls.y() + cpos.x(); i = i + cpos.x())
			{
				for (short h = pos.y(); h < mapborders.y() + cpos.y(); h = h + cpos.y())
				{
					ani.draw(target, vector2d(i + parentpos.x(), h + parentpos.y()), alpha);
				}
			}
			break;
		default:
			ani.draw(target, absp, alpha);
		}

		if (flipped)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(1.0f, 1.0f),
				D2D1::Point2F(
				(float)absp.x(),
				(float)absp.y())));
		}
	}

	void background::update()
	{
		ani.update();
		fx += hspeed;
		fy += vspeed;
	}
}
