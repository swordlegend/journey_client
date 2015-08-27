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
#include "stdfax.h"
#include "node.h"
#include "vector2d.h"

using namespace util;
using namespace std;
using namespace nl;

namespace gameplay
{
	struct foothold
	{
		short prev;
		short next;
		vector2d horizontal;
		vector2d vertical;

		bool iswall() { return horizontal.x() == horizontal.y(); }
		bool isfloor() { return vertical.x() == vertical.y(); }
	};

	class footholdtree
	{
	public:
		footholdtree(node);
		footholdtree() {}
		~footholdtree() {}
		float getgroundbelow(vector2d);
		float nextground(bool, vector2d);
		vector2d getwalls();
		vector2d getborders();
	private:
		map<short, foothold> footholds;
		vector<map<short, short>> platforms;
		vector<short> edgesl;
		vector<short> edgesr;
		foothold ground;
		foothold lwall;
		foothold rwall;
		int lowestg;
	};
}

