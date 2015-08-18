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
#include "mapleitem.h"

namespace gameplay
{
	mapleitem::mapleitem() {}

	mapleitem::~mapleitem() {}

	mapleitem::mapleitem(int id, short c, short p, char typ, bool cs, long uq, long exp, std::string own, short fl)
	{
		itemid = id;
		count = c;
		pos = p;
		type = typ;
		cash = cs;
		uniqueid = uq;
		expire = exp;
		owner = own;
		flag = fl;
	}

	mapleitem::mapleitem(int id, short p, char typ, bool cs, long uq, long exp, std::string name, char lv, short close, char full)
	{
		itemid = id;
		count = 1;
		pos = p;
		type = typ;
		cash = cs;
		uniqueid = uq;
		expire = exp;
		flag = -1;
		petname = name;
		petlevel = lv;
		closeness = close;
		fullness = full;
	}
}
