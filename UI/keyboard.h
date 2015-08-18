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

using namespace std;

namespace io
{
	enum keyaction : char
	{
		KA_EQUIPS = 0,
		KA_INVENTORY = 1,
		KA_SKILL = 3,
		KA_BUDDY = 4,
		KA_WORLDMAP = 5,
		KA_QUEST = 8,
		KA_CHAT1 = 12,
		KA_PARTYSEARCH = 25,
		KA_PICKUP = 50,
		KA_SIT = 51,
		KA_ATTACK = 52,
		KA_NPCCHAT = 53, //these are switched 
		KA_JUMP = 54,	// ^
		KA_GUILD = 17,
		KA_FACE1 = 100,
		KA_FACE7 = 106
	};

	enum keytype : char
	{
		KT_NONE = 0,
		KT_SKILL = 1,
		KT_ITEM = 2,
		KT_CASH = 3,
		KT_MENU = 4,
		KT_ACTION = 5,
		KT_FACE = 6,
		KT_MACRO = 8
	};

	class keyboard
	{
	private:
		map<char, pair<keytype, int>> keymap;
		char* keytable;
	public:
		keyboard();
		~keyboard();
		void init(map<char, pair<char, int>>);
		void addmacros(map<string, pair<char, vector<int>>>);
		pair<keytype, int> getaction(char);
	};
}

