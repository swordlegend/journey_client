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
#include "moveobject.h"

namespace gameplay
{
	moveobject::moveobject(vector2d pos)
	{
		fx = static_cast<float>(pos.x());
		fy = static_cast<float>(pos.y());
		hspeed = 0;
		vspeed = 0;
	}

	void moveobject::moveto(vector2d dst, int speed)
	{
		vector2d delta = dst - vector2d(static_cast<int>(fx), static_cast<int>(fy));
		hspeed = speed * static_cast<float>(delta.x()) / 60;
		vspeed = speed * static_cast<float>(delta.y()) / 60;
		destination = dst;
	}

	void moveobject::moveabsx(int xdst, int speed)
	{
		int delta = xdst - static_cast<int>(fx);
		hspeed = speed * static_cast<float>(delta) / 60;
		destination = vector2d(xdst, static_cast<int>(fy));
	}

	void moveobject::moveabsy(int ydst, int speed)
	{
		int delta = ydst - static_cast<int>(fy);
		vspeed = speed * static_cast<float>(delta) / 60;
		destination = vector2d(static_cast<int>(fx), ydst);
	}

	bool moveobject::update()
	{
		fx += hspeed;
		fy += vspeed;

		if (abs(destination.x() - static_cast<int>(fx)) <= abs(hspeed) * 2)
		{
			hspeed = 0;
			fx = static_cast<float>(destination.x());
		}

		if (abs(destination.y() - static_cast<int>(fy)) <= abs(hspeed) * 2)
		{
			vspeed = 0;
			fy = static_cast<float>(destination.y());
		}

		return hspeed == 0 && vspeed == 0;
	}

	vector2d moveobject::getposition()
	{
		return vector2d(static_cast<int>(fx), static_cast<int>(fy));
	}
}
