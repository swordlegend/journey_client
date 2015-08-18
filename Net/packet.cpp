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
#include "packet.h"

namespace net
{
	packet::packet()
	{
		opcode = 0;
	}

	packet::packet(short opc)
	{
		writesh(opc);
	}

	packet::packet(char* recv, int len)
	{
		for (int i = len - 1; i >= 0; i--)
		{
			bytes.push_back(recv[i]);
		}
		opcode = readshort();
	}

	packet::~packet() {}

	template<class T>
	T packet::readbytes(int size)
	{
		if (bytes.size() < size)
			throw new runtime_error("packet error: stack underflow");
		long all = 0;
		for (int i = 0; i < size; i++)
		{
			int num = ((byte) bytes.back()) * pow(256, i);
			bytes.pop_back();
			all = all + num;
		}
		auto ret = all;
		return ret;
	}

	char packet::readbyte()
	{
		return readbytes<char>(1);
	}

	bool packet::readbool()
	{
		return readbytes<char>(1) == 1;
	}

	short packet::readshort()
	{
		return readbytes<short>(2);
	}

	int packet::readint()
	{
		return readbytes<int>(4);
	}

	long packet::readlong()
	{
		return readbytes<long>(8);
	}

	string packet::readascii()
	{
		short length = readshort();
		if (length > bytes.size())
			throw new runtime_error("packet error: stack underflow");
		string ret;
		for (char i = 0; i < length; i++)
		{
			ret.push_back(bytes.back());
			bytes.pop_back();
		}
		return ret;
	}

	string packet::readntascii()
	{
		string ret;

		char letter = bytes.back();
		bytes.pop_back();
		while (letter != '\0')
		{
			ret.push_back(letter);
			letter = bytes.back();
			bytes.pop_back();
		}
		return ret;
	}

	string packet::readpadascii(char length)
	{
		if (length > bytes.size())
			throw new runtime_error("packet error: stack underflow");
		string ret;
		for (char i = 0; i < length; i++)
		{
			ret.push_back(bytes.back());
			bytes.pop_back();
		}
		return ret;
	}

	vector2d packet::readpoint()
	{
		short x = readbytes<short>(2);
		short y = readbytes<short>(2);
		return vector2d(x, y);
	}

	char* packet::getbytes()
	{
		return bytes.data();
	}


	int packet::length()
	{
		return bytes.size();
	}


	short packet::getopc()
	{
		return opcode;
	}

	void packet::writebytes(char* recv, int length)
	{
		for (int i = length - 1; i >= 0; i--)
		{
			bytes.push_back(recv[i]);
		}
	}

	void packet::writeint(int integer)
	{
		for (int i = 0; i < 4; i++)
		{
			bytes.push_back(static_cast<char>(integer));
			integer = integer >> 8;
		}
	}

	void packet::writesh(short sh)
	{
		for (int i = 0; i < 2; i++)
		{
			bytes.push_back(static_cast<char>(sh));
			sh = sh >> 8;
		}
	}

	void packet::writebl(bool bl)
	{
		bytes.push_back(bl);
	}

	void packet::writech(char ch)
	{
		bytes.push_back(ch);
	}

	void packet::writestr(string str)
	{
		writesh(str.length());
		for (int i = 0; i < str.length(); i++)
		{
			bytes.push_back(str[i]);
		}
	}
}