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
#include "laddersropes.h"

namespace gameplay
{
	laddersropes::laddersropes(node src)
	{
		for (node lrnode = src.begin(); lrnode != src.end(); ++lrnode)
		{
			ladderrope lr;
			lr.vertical = vector2d(lrnode["y1"], lrnode["y2"]);
			lr.x = lrnode["x"];
			lr.ladder = lrnode["l"].get_bool();
			landr.push_back(lr);
		}
	}

	ladderrope laddersropes::getlr(vector2d pos)
	{
		for (vector<ladderrope>::iterator lrit = landr.begin(); lrit != landr.end(); ++lrit)
		{
			if (vector2d(pos.x() - 25, pos.x() + 25).contains(lrit->x) && lrit->vertical.contains(pos.y()))
			{
				return *lrit;
			}
		}

		return ladderrope();
	}
}
