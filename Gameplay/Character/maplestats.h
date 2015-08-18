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

namespace gameplay
{
	class maplestats
	{
	public:
		int id;
		std::string name;
		bool female;
		char skin;
		int face;
		int hair;
		std::vector<long> petids;
		short level;
		short job;
		short str;
		short dex;
		short int_;
		short luk;
		short hp;
		short mhp;
		short mp;
		short mmp;
		short ap;
		short sp;
		int exp;
		short fame;
		int map;
		char spawnp;
		maplestats();
		maplestats(int, std::string, bool, char, int, int, std::vector<long>, short, short, short, short, short, short, short, short, short, short, short, short, int, short, int, char);
		~maplestats();
	};
}

