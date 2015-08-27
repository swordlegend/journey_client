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
#include "vector2d.h"
#include "node.h"

using namespace std;
using namespace util;
using namespace nl;

namespace gameplay
{
	class mapinfo
	{
	public:
		mapinfo(int, node, vector2d, vector2d);
		mapinfo() {}
		~mapinfo() {}
		int getid() { return mapid; }
		vector2d getwalls() { return mapwalls; }
		vector2d getborders() { return mapborders; }
		string getbgm() { return bgm; }
	private:
		int mapid;
		int fieldlimit;
		bool cloud;
		string bgm;
		string mapdesc;
		string mapname;
		string streetname;
		string mapmark;
		bool swim;
		bool town;
		bool hideminimap;
		vector2d mapwalls;
		vector2d mapborders;
	};
}

