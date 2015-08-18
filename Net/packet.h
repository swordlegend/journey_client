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
#include "vector2d.h"

using namespace std;
using namespace util;

namespace net
{
	class packet
	{
	private:
		vector<char> bytes;
		short opcode;
	public:
		packet();
		packet(short);
		packet(char*, int);
		~packet();
		template <class T>
		T readbytes(int);
		char readbyte();
		bool readbool();
		short readshort();
		int readint();
		long readlong();
		string readascii();
		string readntascii();
		string readpadascii(char);
		vector2d readpoint();
		char* getbytes();
		int length();
		short getopc();
		void writebytes(char*, int);
		void writebl(bool);
		void writech(char);
		void writesh(short);
		void writeint(int);
		void writelg(long);
		void writestr(string);

		template <class T>
		vector<T> read(short);
		template <class T1, class T2>
		pair<T1, T2> read();
		template <class K, class V>
		map<K, V> read(short);
	};
}
