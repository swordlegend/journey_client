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
#include "maplestats.h"

namespace gameplay
{
	maplestats::maplestats(int i, std::string nm, bool fem, short sk, int fc, int hr, int ep, short fm, map<maplestat, short> st, pair<int, char> wi, vector<long> pets)
	{
		id = i;
		name = nm;
		female = fem;
		skin = sk;
		face = fc;
		hair = hr;
		exp = ep;
		fame = fm;
		stats = st;
		spawninfo = wi;
		petids = pets;
	}

	short maplestats::getstat(maplestat s) 
	{ 
		return (stats.count(s)) ? stats[s] : 0;
	}
}
