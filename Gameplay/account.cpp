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
#include "account.h"


namespace gameplay
{
	account::account() {}

	account::~account() {}

	void account::init(int id, std::string name, char gm, bool fem, bool mute, short pn)
	{
		accid = id;
		accname = name;
		gmlevel = gm;
		female = fem;
		muted = mute;
		pin = pn;
		selected = 0;
	}

	void account::addchars(std::vector<maplechar> m, char p, char s)
	{
		maplechars = m;
		pic = p;
		slots = s;
	}

	int account::charcount()
	{
		return maplechars.size();
	}

	char account::getslots()
	{
		return slots;
	}

	vector<maplechar>* account::getchars()
	{
		return &maplechars;
	}

	maplechar* account::getplayer()
	{
		return &maplechars[selected];
	}

	void account::selectchar(int cid)
	{
		for (vector<maplechar>::iterator mchar = maplechars.begin(); mchar != maplechars.end(); mchar++)
		{
			if (mchar->getstats()->getid() == cid)
				break;
			selected++;
		}
	}

	void account::clear()
	{
		maplechars.clear();
	}
}
