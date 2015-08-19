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
#include "footholdtree.h"

namespace gameplay
{
	footholdtree::footholdtree(nl::node source)
	{
		for (nl::node basef = source.begin(); basef != source.end(); ++basef)
		{
			for (nl::node midf = basef.begin(); midf != basef.end(); ++midf)
			{
				for (nl::node lastf = midf.begin(); lastf != midf.end(); ++lastf)
				{
					short id = static_cast<short>(stoi(lastf.name()));
					footholds[id] = struct foothold();
					footholds[id].prev = static_cast<short>(lastf.resolve("prev").get_integer());
					footholds[id].next = static_cast<short>(lastf.resolve("next").get_integer());
					footholds[id].horizontal = vector2d(static_cast<int>(lastf.resolve("x1").get_integer()), static_cast<int>(lastf.resolve("x2").get_integer()));
					footholds[id].vertical = vector2d(static_cast<int>(lastf.resolve("y1").get_integer()), static_cast<int>(lastf.resolve("y2").get_integer()));
				}
			}
		}
	}

	void footholdtree::addfoothold(short id, short pr, short nx, vector2d hr, vector2d vr)
	{
		footholds[id] = struct foothold();
		footholds[id].prev = pr;
		footholds[id].next = nx;
		footholds[id].horizontal = hr;
		footholds[id].vertical = vr;
	}

	float footholdtree::getgroundbelow(vector2d pos)
	{
		struct foothold ret = footholds.begin()->second;
		int ycomp = 65536;
		pos = pos - vector2d(0, 5);
		for (map<short, struct foothold>::iterator ftit = footholds.begin(); ftit != footholds.end(); ftit++)
		{
			struct foothold cur = ftit->second;
			if (cur.horizontal.contains(pos.x()) && ycomp > cur.vertical.y() && cur.vertical.y() > pos.y() && cur.vertical.straight())
			{
				ret = cur;
				ycomp = cur.vertical.y();
			}
		}

		ground = ret;

		if (ret.vertical.straight())
		{
			return static_cast<float>(ret.vertical.x());
		}
		else
		{
			return static_cast<float>((pos.x() - ret.horizontal.x()) * sin((ret.horizontal.y() - ret.horizontal.x()) / (ret.vertical.y() - ret.vertical.x())));
		}
	}

	float footholdtree::nextground(bool left, vector2d pos)
	{
		short nextid;
		if (left && pos.x() < ground.horizontal.x())
		{
			nextid = ground.prev;
		}
		else if (!left && pos.x() > ground.horizontal.y())
		{
			nextid = ground.next;
		}

		if (nextid > 0)
		{
			ground = footholds[nextid];

			if (ground.vertical.straight())
			{
				return static_cast<float>(ground.vertical.x());
			}
			else
			{
				return static_cast<float>((pos.x() - ground.horizontal.x()) * sin((ground.horizontal.y() - ground.horizontal.x()) / (ground.vertical.y() - ground.vertical.x())));
			}
		}
		else
		{
			return getgroundbelow(pos);
		}
	}
}
