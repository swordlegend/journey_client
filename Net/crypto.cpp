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
#include "crypto.h"

namespace net
{
	crypto::crypto()
	{
		recvIv = new char[4];
		sendIv = new char[4];
		version = 0;
		localisation = 0;
		jrkey  = new char[13];
	}

	crypto::crypto(char ver, char* recv, char* send, char loc)
	{
		recvIv = recv;
		sendIv = send;
		version = ver;
		localisation = loc;
		jrkey = new char[13] { 33, 101, 54, 27, 11, 7, 78, 62, 119, 120, 47, 85, 93 };
	}

	crypto::~crypto() {}

	char* crypto::sendencrypt(char* bytes, int length)
	{
		char* ret = new char[length + 4];

		int a = (sendIv[3] << 8) | sendIv[2];
		char* header = new char[4];
		a ^= (version);
		int b = a ^ length;
		header[0] = (char)(a % 0x100);
		header[1] = (char)(a / 0x100);
		header[2] = (char)(b % 0x100);
		header[3] = (char)(b / 0x100);
		memcpy(ret, header, 4);

		memcpy(ret + 4, bytes, length);

		sendIv = updateiv(sendIv);
		return ret;
	}

	char* crypto::recvdecrypt(char* bytes, int length)
	{
		int a = (recvIv[3] << 8) | recvIv[2];
		char* header = new char[4];
		a ^= -(version + 1);
		int b = a ^ length;
		header[0] = (char)(a % 0x100);
		header[1] = (char)((a - header[0]) / 0x100);
		header[2] = (char)(b ^ 0x100);
		header[3] = (char)((b - header[2]) / 0x100);

		recvIv = updateiv(recvIv);
		return header;
	}

	char* crypto::updateiv(char* lastiv)
	{
		char* jrbytes = new char[4] { 69, 42, 13, 124 };
		for (byte i = 0; i < 4; i++) {
			int value = jrbytes[i] + jrkey[std::abs(lastiv[3 - i]) / 12];
			value = value - lastiv[i];
			jrbytes[i] = (char)std::abs((char)value);
			if (jrbytes[i] < 0)
				jrbytes[i] += 128;
		}
		return jrbytes;
	}
}