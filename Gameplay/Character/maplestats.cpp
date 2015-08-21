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
	maplestats::maplestats(){}
	maplestats::~maplestats(){}

	maplestats::maplestats(int i, std::string nm, bool fem, char sk, int fc, int hr, std::vector<long> pets, short lv, short jb, short st, short dx, short it, short lk, short h, short mh, short m, short mm, short ra, short rs, int ep, short fm, int mi, char spa)
	{
		id = i;
		name = nm;
		female = fem;
		skin = sk;
		face = fc;
		hair = hr;
		petids = pets;
		level = lv;
		job = jb;
		str = st;
		dex = dx;
		int_ = it;
		luk = lk;
		hp = h;
		mhp = mh;
		mp = m;
		mmp = mm;
		ap = ra;
		sp = rs;
		exp = ep;
		fame = fm;
		map = mi;
		spawnp = spa;
		basedamage = 0;
	}
}
