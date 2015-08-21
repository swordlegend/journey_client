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
#include "mapleequip.h"

namespace gameplay
{
	mapleequip::mapleequip(short ps, char typ, int id, bool cs, long uq, long expi, char slt, char lv, map<equipstat, short> sta, string own, short fl, char ilv, short iex, int vic)
	{
		pos = ps;
		count = 1;
		type = typ;
		itemid = id;
		cash = cs;
		uniqueid = uq;
		expire = expi;
		slots = slt;
		level = lv;
		stats = sta;
		owner = own;
		flag = fl;
		itemexp = iex;
		itemlevel = ilv;
		vicious = vic;
	}
}
