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
#include "mapportals.h"

namespace gameplay
{
	void mapportals::clear()
	{
		portals.clear();
	}

	void mapportals::addportal(char id, portal toadd)
	{
		portals[id] = toadd;
	}

	vector2d mapportals::getspawnpoint(char id)
	{
		return portals[id].getposition();
	}

	vector2d mapportals::getspawnpoint(string pname)
	{
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			if (pit->second.getname() == pname)
				return pit->second.getposition();
		}
		return portals[0].getposition();
	}

	pair<int, string> mapportals::getportal(vector2d playerpos)
	{
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			if (colliding(make_pair(playerpos, vector2d(50, 80)), make_pair(pit->second.getposition(), pit->second.getdimension())) && pit->second.gettype() != PT_WARP)
				return pit->second.getwarpinfo();
		}
		return make_pair(-1, "");
	}

	void mapportals::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			pit->second.draw(target, viewpos);
		}
	}

	void mapportals::update(vector2d playerpos)
	{
		for (map<char, portal>::iterator pit = portals.begin(); pit != portals.end(); pit++)
		{
			pit->second.settouch(colliding(make_pair(playerpos, vector2d(50, 80)), make_pair(pit->second.getposition(), pit->second.getdimension())));
			pit->second.update();
		}
	}
}
