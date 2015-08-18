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
#include "maplechar.h"

using namespace std;

namespace gameplay
{
	class account
	{
	private:
		vector<maplechar> maplechars;
		int accid;
		string accname;
		char gmlevel;
		bool female;
		bool muted;
		short pin;
		char pic;
		char slots;
		char selected;
	public:
		account();
		~account();
		void init(int, string, char, bool, bool, short);
		void addchars(vector<maplechar>, char, char);
		int charcount();
		char getslots();
		void selectchar(int);
		void clear();
		maplechar* getplayer();
		vector<maplechar>* getchars();
	};
}
