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
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include "packet.h"
#include "crypto.h"
#include "packethandler.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace net
{
	const int BUFLEN  = 10240;

	class session
	{
	private:
		SOCKET sock;
		crypto encrypter;
		packet curp;
		packethandler packet_h;
		mutex sendlock;
		char buf[BUFLEN];
		int bufferpos;
		short p_length;
		bool active;
	public:
		session() {}
		~session() {}
		int init();
		int init(const char*, const char*);
		int reconnect(char*, short);
		int dispatch(packet);
		int receive();
		void close();
		void process(char*, int);
	};
}

