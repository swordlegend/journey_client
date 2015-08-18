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
#include "inventory.h"

namespace gameplay
{
	inventory::inventory() {}

	inventory::~inventory() {}

	inventory::inventory(char* s, map<short, mapleequip> eq, map<short, mapleequip> eqc, map<short, mapleequip> e, map<char, mapleitem> ui, map<char, mapleitem> si, map<char, mapleitem> ei, map<char, mapleitem> ci)
	{
		slots = s;
		equipped = eq;
		equippedcash = eqc;
		equips = e;
		useitems = ui;
		setupitems = si;
		etcitems = ei;
		cashitems = ci;
	}

	map<short, mapleequip>* inventory::getequipped()
	{
		return &equipped;
	}

	map<short, mapleequip>* inventory::getequippedcash()
	{
		return &equippedcash;
	}
}
