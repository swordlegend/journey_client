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
#include "camera.h"
#include "Journey.h"

namespace gameplay
{
	camera::camera()
	{
		posx = 0;
		posy = 0;
	}

	void camera::update(vector2d playerpos)
	{
		playerpos = vector2d(408, 416) - playerpos;

		if (playerpos.x() - static_cast<int>(posx) == 0 && playerpos.y() - static_cast<int>(posy) == 0)
		{
			return;
		}
		else
		{
			movex = (playerpos.x() - posx) / 40;
			movey = (playerpos.y() - posy) / 40;
		}
		

		posx = posx + movex;
		posy = posy + movey;

		if (abs(playerpos.x() - posx) < 1)
		{
			posx = static_cast<float>(playerpos.x());
			movex = 0;
		}
		if (abs(playerpos.y() - posy) < 1)
		{
			posy = static_cast<float>(playerpos.y());
			movey = 0;
		}
	}

	void camera::setposition(vector2d pos)
	{
		pos = vector2d(408, 416) - pos;

		posx = static_cast<float>(pos.x());
		posy = static_cast<float>(pos.y());
	}

	void camera::setbounds(vector2d hbd, vector2d vbd)
	{
		hbounds = vector2d(-hbd.x(), -hbd.y());
		vbounds = vector2d(-vbd.x(), -vbd.y() - 60);
	}

	vector2d camera::getposition()
	{
		int retx;
		int rety;

		if (posx > hbounds.x() || hbounds.length() < 816)
			retx = hbounds.x();
		else if (posx < hbounds.y() + 816)
			retx = hbounds.y() + 816;
		else
			retx = static_cast<int>(posx);

		if (posy < vbounds.y() + 624 || vbounds.length() < 624)
			rety = vbounds.y() + 624;
		else if (posy > vbounds.x())
			rety = vbounds.x();
		else
			rety = static_cast<int>(posy);

		return vector2d(retx, rety);
	}
}
