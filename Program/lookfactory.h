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
#include "maplelook.h"

using namespace std;
using namespace gameplay;

namespace program
{
	class lookfactory
	{
	public:
		lookfactory() {}
		~lookfactory() {}
		void loadcharlook(maplelook*);
	private:
		void initbodyinfo();
		void initfaceinfo();
		void addbodytype(char);
		void addhairstyle(int);
		void addface(int);
		void addclothes(int);
		map<char, charsprites> bodytypes;
		map<int, charsprites> faces;
		map<int, charsprites> hairstyles;
		map<int, charsprites> clothes;
		map<string, map<char, short>> facedelays;
		map<string, map<char, short>> bodydelays;
		map<string, map<char, pair<string, char>>> bodyactions;
		map<string, map<char, map<charlayers, map<string, vector2d>>>> bodyheadmap;
	};
}

