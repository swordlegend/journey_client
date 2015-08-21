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
#include "mapleitem.h"

using namespace std;

namespace gameplay
{
	enum equipstat : char
	{
		ES_STR,
		ES_DEX,
		ES_INT,
		ES_LUK,
		ES_HP,
		ES_MP,
		ES_WATK,
		ES_MAGIC,
		ES_WDEF,
		ES_MDEF,
		ES_ACC,
		ES_AVOID,
		ES_HANDS,
		ES_SPEED,
		ES_JUMP
	};

	class mapleequip : mapleitem
	{
	private:
		char slots;
		char level;
		map<equipstat, short> stats;
		string owner;
		short flag;
		char itemlevel;
		short itemexp;
		int vicious;
	public:
		mapleequip() {}
		~mapleequip() {}
		mapleequip(short, char, int, bool, long, long, char, char, map<equipstat, short>,string, short, char, short, int);
		short getstat(equipstat es) { return stats[es]; }
	};
}

