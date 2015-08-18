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
#include "keyboard.h"

namespace io
{
	keyboard::keyboard() 
	{
		keytable = new char[90] 
		{
			0, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, // number keys, up to key 10
			0, 0, 0, 0, 0, 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', 0, 0, //first letter row, up to key 27
			VK_CONTROL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0, 0, 0, 0,  //second row, up to 41
			VK_SHIFT, 0, 'Y', 'X', 'C', 'V', 'B', 'N', 'M', 0, 0, 0, 0, 0, VK_MENU, VK_SPACE, 0, //third row, up to 58
			VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
			VK_HOME, 0, VK_PRIOR, 0, 0, 0, 0, 0, 0, VK_END, 0, VK_NEXT, VK_INSERT, VK_DELETE, 0, 0, 0
		};
	}

	keyboard::~keyboard() {}

	void keyboard::init(map<char, pair<char, int>> keys)
	{
		for (map<char, pair<char, int>>::iterator keyit = keys.begin(); keyit != keys.end(); keyit++)
		{
			char kcode = keytable[keyit->first];

			keytype ktype = static_cast<keytype>(keyit->second.first);
			int action = keyit->second.second;

			keymap[kcode] = make_pair(ktype, action);
		}
	}

	void keyboard::addmacros(map<string, pair<char, vector<int>>> mac)
	{

	}

	pair<keytype, int> keyboard::getaction(char keycode)
	{
		return keymap[keycode];
	}
}
