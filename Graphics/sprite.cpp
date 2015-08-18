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
#include "sprite.h"

namespace graphics
{
	sprite::sprite(animation ani, vector2d pos)
	{
		anim = ani;
		position = pos;
		visible = true;
		flipped = false;
	}

	sprite::sprite(animation ani, vector2d pos, bool vis, bool flp)
	{
		anim = ani;
		position = pos;
		visible = vis;
		flipped = flp;
	}

	void sprite::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		if (visible)
		{
			if (flipped)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(-1.0f, 1.0f),
					D2D1::Point2F(
					(float)(parentpos + position).x(),
					(float)(parentpos + position).y())));
			}

			anim.draw(target, position + parentpos);

			if (flipped)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(1.0f, 1.0f),
					D2D1::Point2F(
					(float)(parentpos + position).x(),
					(float)(parentpos + position).y())));
			}
		}
	}
}