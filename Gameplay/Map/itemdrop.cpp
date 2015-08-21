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
#include "itemdrop.h"

namespace gameplay
{
	itemdrop::itemdrop(int id, bool mes, vector2d pos, vector2d dest, int o, int own, char put, bool pld)
	{
		if (!meso)
		{
			ico = icon(id, true);
		}
		position = pos;
		destination = dest;
		oid = o;
		itemid = id;
		owner = own;
		meso = mes;
		pickuptype = put;
		playerdrop = pld;
		alpha = 1.0f;
		pickedup = false;
		vspeed = 0;

		if (position == destination)
		{
			borders = vector2d(position.y() - 10, position.y() + 10);
			fy = static_cast<float>(position.y());
			floating = true;
		}
		else
		{
			borders = vector2d();
			fy = 0;
			floating = false;
		}
	}

	void itemdrop::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		if (floating)
		{
			fy = fy + vspeed;
			position = vector2d(position.y(), static_cast<int>(fy));
		}

		if (!meso)
		{
			ico.draw(viewpos, alpha);
		}
	}

	bool itemdrop::update()
	{
		if (floating && !pickedup)
		{
			vspeed = (position.y() > borders.center()) ? -2.0f : 2.0f;
			vspeed += (borders.center() - position.y());
		}
		else
		{
			position = position + ((destination - position) / 5);

			if ((destination - position).length() < 5)
			{
				position = destination;
				borders = vector2d(position.y() - 10, position.y() + 10);
				fy = static_cast<float>(position.y());
				floating = true;
			}
		}

		if (pickedup)
		{
			alpha = alpha - 0.05f;
			if (alpha < 0.05f)
			{
				return true;
			}
		}
		return false;
	}
}
