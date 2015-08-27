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
	footholdtree::footholdtree(node source)
	{
		for (node basef = source.begin(); basef != source.end(); ++basef)
		{
			for (node midf = basef.begin(); midf != basef.end(); ++midf)
			{
				map<short, short> platform;

				for (node lastf = midf.begin(); lastf != midf.end(); ++lastf)
				{
					short id = static_cast<short>(stoi(lastf.name()));
					footholds[id] = struct foothold();
					footholds[id].prev = static_cast<short>(lastf["prev"]);
					footholds[id].next = static_cast<short>(lastf["next"]);
					footholds[id].horizontal = vector2d(static_cast<int>(lastf["x1"]), static_cast<int>(lastf["x2"]));
					footholds[id].vertical = vector2d(static_cast<int>(lastf["y1"]), static_cast<int>(lastf["y2"]));

					if (footholds[id].prev == 0)
					{
						edgesl.push_back(id);
					}
					else if (footholds[id].next == 0)
					{
						edgesr.push_back(id);
					}

					/*if (footholds[id].isfloor())
					{
						short y = footholds[id].vertical.x();

						for (short i = footholds[id].horizontal.x(); i <= footholds[id].horizontal.y(); i++)
						{
							platform[i] = y;
						}
					}
					else 
					{
						vector2d slope = footholds[id].vertical;
						short distance = footholds[id].horizontal.y() - footholds[id].horizontal.x();

						if (slope.x() > slope.y())
						{
							float step = static_cast<float>((slope.x() - slope.y())) / distance;

							for (short i = footholds[id].horizontal.x(); i <= footholds[id].horizontal.y(); i++)
							{
								platform[i] = slope.x() - static_cast<int>(i * step);
							}
						}
						else
						{
							float step = static_cast<float>((slope.y() - slope.x())) / distance;

							for (short i = footholds[id].horizontal.x(); i <= footholds[id].horizontal.y(); i++)
							{
								platform[i] = slope.x() + static_cast<int>(i * step);
							}
						}
					}*/
				}

				platforms.push_back(platform);
			}
		}

		lowestg = -65536;
		for (vector<map<short, short>>::iterator pfit = platforms.begin(); pfit != platforms.end(); ++pfit)
		{
			for (map<short, short>::iterator yit = pfit->begin(); yit != pfit->end(); ++yit)
			{
				if (yit->second > lowestg)
				{
					lowestg = yit->second;
				}
			}
		}
	}

	float footholdtree::getgroundbelow(vector2d pos)
	{
		foothold ret;
		float ycomp = 65536;
		pos = pos - vector2d(0, 5);
		for (map<short, foothold>::iterator ftit = footholds.begin(); ftit != footholds.end(); ftit++)
		{
			if (ftit->second.horizontal.contains(pos.x()))
			{
				float y;
				if (ftit->second.isfloor())
				{
					y = static_cast<float>(ftit->second.vertical.x());
				}
				else
				{
					float step = static_cast<float>((ftit->second.vertical.y() - ftit->second.vertical.x())) / (ftit->second.horizontal.y() - ftit->second.horizontal.x());
					y = static_cast<float>(ftit->second.vertical.x() + (step * (pos.x() - ftit->second.horizontal.x())));
				}

				if (ycomp > y && y > pos.y())
				{
					ret = ftit->second;
					ycomp = y;
				}
			}
		}

		ground = ret;
		return ycomp;
		

		/*int closest = lowestg;
		pos = pos - vector2d(0, 5);

		for (vector<map<short, short>>::iterator pfit = platforms.begin(); pfit != platforms.end(); ++pfit)
		{
			if (pfit->count(pos.x()))
			{
				int py = pfit->at(pos.x());

				if (py <= closest && py >= pos.y())
				{
					closest = py;
				}
			}
		}

		return static_cast<float>(closest);*/
	}

	float footholdtree::nextground(bool left, vector2d pos)
	{
		//return getgroundbelow(pos);

		short nextid = -1;
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

			if (ground.isfloor())
			{
				return static_cast<float>(ground.vertical.x());
			}
			else
			{
				float step = static_cast<float>((ground.vertical.y() - ground.vertical.x())) / (ground.horizontal.y() - ground.horizontal.x());
				return static_cast<float>(ground.vertical.x() + (step * (pos.x() - ground.horizontal.x())));
			}
		}
		else
		{
			return getgroundbelow(pos);
		}
	}

	vector2d footholdtree::getwalls()
	{
		int leftw = 65536;
		for (vector<short>::iterator fhit = edgesl.begin(); fhit != edgesl.end(); ++fhit)
		{
			foothold edge = footholds[*fhit];
			leftw = min(edge.horizontal.x(), leftw);
		}

		int rightw = -65536;
		for (vector<short>::iterator fhit = edgesr.begin(); fhit != edgesr.end(); ++fhit)
		{
			foothold edge = footholds[*fhit];
			rightw = max(edge.horizontal.x(), rightw);
		}

		return vector2d(leftw + 15, rightw);
	}

	vector2d footholdtree::getborders()
	{
		int botb = -65536;
		for (map<short, foothold>::iterator fhit = footholds.begin(); fhit != footholds.end(); ++fhit)
		{
			int ycomp = max(fhit->second.vertical.x(), fhit->second.vertical.y());
			botb = max(ycomp, botb);
		}

		int topb = 65536;
		for (map<short, foothold>::iterator fhit = footholds.begin(); fhit != footholds.end(); ++fhit)
		{
			int ycomp = min(fhit->second.vertical.x(), fhit->second.vertical.y());
			topb = min(ycomp, topb);
		}

		return vector2d(topb - 400, botb + 400);
	}
}
