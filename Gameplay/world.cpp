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
#include "world.h"

namespace gameplay
{
	world::~world() {}

	world::world()
	{
		id = -1;
		flag = 0;
		channels = 0;
		chloads = new char[channels];
		name = "";
		eventmsg = "";
	}

	world::world(char i, std::string n, char f, std::string m, char ch, char* chl)
	{
		id = i;
		flag = f;
		channels = ch;
		chloads = chl;
		name = n;
		eventmsg = m;
	}

	char world::getid()
	{
		return id;
	}

	char world::getchannels()
	{
		return channels;
	}

	char* world::getchloads()
	{
		return chloads;
	}
}
