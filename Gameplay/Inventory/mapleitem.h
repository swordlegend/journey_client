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
	class mapleitem
	{
	public:
		mapleitem();
		~mapleitem();
		mapleitem(int, short, short, char, bool, long, long, std::string, short);
		mapleitem(int, short, char, bool, long, long, std::string, char, short, char);
		int getid() { return itemid; }
	protected:
		int itemid;
		short count;
		short pos;
		char type;
		bool cash;
		long uniqueid;
		long expire;
		std::string owner;
		short flag;
		std::string petname;
		char petlevel;
		short closeness;
		char fullness;
	};
}

