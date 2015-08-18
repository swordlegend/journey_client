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
#include "collision.h"

namespace util
{
	bool colliding(vector2d a, vector2d b, vector2d dimb)
	{
		return (a.x() > b.x() && a.x() < b.x() + dimb.x()) && (a.y() > b.y() && a.y() < b.y() + dimb.y());
	}

	bool colliding(vector2d a, pair<vector2d, vector2d> b)
	{
		return (a.x() > b.first.x() && a.x() < b.first.x() + b.second.x()) && (a.y() > b.first.y() && a.y() < b.first.y() + b.second.y());
	}

	bool colliding(vector2d a, pair<vector2d, vector2d> b, vector2d c)
	{
		return (a.x() > b.first.x() + c.x() && a.x() < b.first.x() + c.x() + b.second.x()) && (a.y() > b.first.y() + c.y() && a.y() < b.first.y() + c.y() + b.second.y());
	}

	bool colliding(pair<vector2d, vector2d> a, pair<vector2d, vector2d> b)
	{
		return (a.first.x() + a.second.x() > b.first.x() && a.first.x() < b.first.x() + b.second.x()) && (a.first.y() + a.second.y() > b.first.y() && a.first.y() < b.first.y() + b.second.y());
	}
}