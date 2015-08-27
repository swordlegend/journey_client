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
#include "session.h"

namespace net
{
	int session::reconnect(char* iaddr, short port)
	{
		close();

		char* portstr = new char[5];
	    string temp = to_string(port);
		portstr[0] = temp[0];
		portstr[1] = temp[1];
		portstr[2] = temp[2];
		portstr[3] = temp[3];
		portstr[4] = '\0';

		return init(iaddr, portstr);
	}

	int session::init()
	{
		return init("localhost", "8484");
	}

	int session::init(const char* iaddr, const char* port)
	{
		WSADATA wsa_info;
		sock = INVALID_SOCKET;

		struct addrinfo *addr_info = NULL;
		struct addrinfo *ptr = NULL;
		struct addrinfo hints;

		int result;

		result = WSAStartup(MAKEWORD(2, 2), &wsa_info);
		if (result != 0) {
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		if (iaddr != "localhost")
		{
			string temp = "";
			for (char i = 0; i < 4; i++)
			{
				temp.append(to_string(iaddr[i]));
				if (i < 3)
					temp.append(".");
			}

			if (temp == "127.0.0.1")
				temp = "localhost";

			iaddr = temp.c_str();
		}

		result = getaddrinfo(iaddr, port, &hints, &addr_info);
		if (result != 0) {
			WSACleanup();
			return 1;
		}

		for (ptr = addr_info; ptr != NULL; ptr = ptr->ai_next) {

			sock = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (sock == INVALID_SOCKET) {
				WSACleanup();
				return 1;
			}

			result = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (result == SOCKET_ERROR) {
				closesocket(sock);
				sock = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(addr_info);

		if (sock == INVALID_SOCKET) {
			WSACleanup();
			return 1;
		}

		char recvbuf[32];

		result = recv(sock, recvbuf, 32, 0);
		if (result != 16) {
			WSACleanup();
			return 2;
		}
		packet rcv = packet(recvbuf, result);
		char version = static_cast<char>(rcv.readshort());
		if (version != 83)
		{
			WSACleanup();
			return 2;
		}

		char* recvIv = new char[4];
		char* sendIv = new char[4];
		char localisation;
		rcv.readshort();
		rcv.readbyte();
		sendIv[0] = rcv.readbyte();
		sendIv[1] = rcv.readbyte();
		sendIv[2] = rcv.readbyte();
		sendIv[3] = rcv.readbyte();
		recvIv[0] = rcv.readbyte();
		recvIv[1] = rcv.readbyte();
		recvIv[2] = rcv.readbyte();
		recvIv[3] = rcv.readbyte();
		localisation = rcv.readbyte();
		encrypter = crypto(version, recvIv, sendIv, localisation);

		u_long iMode = 1;
		ioctlsocket(sock, FIONBIO, &iMode);
		active = true;

		return 0;
	}


	int session::dispatch(packet tosend)
	{
		sendlock.lock();
		char* bytes = encrypter.sendencrypt(tosend.getbytes(), tosend.length());
		int result = send(sock, bytes, tosend.length() + 4, 0);
		sendlock.unlock();

		if (result == SOCKET_ERROR) {
			int nError = WSAGetLastError();
			close();
			return 3;
		}
		return 0;
	}


	int session::receive()
	{
		if (active)
		{
			int result = recv(sock, buf, BUFLEN, 0);

			int nError = WSAGetLastError();
			if (nError != WSAEWOULDBLOCK&&nError != 0)
			{
				close();
				return 2;
			}

			if (result > 0)
			{
				process(buf, result);
			}
		}
		return 0;
	}

	void session::process(char* buffer, int result)
	{
		if (p_length == 0)
		{
			//char* recv = encrypter.recvdecrypt(buffer, result);
			int length = (byte)buffer[0] | (byte)buffer[1] << 8 | (byte)buffer[2] << 16 | (byte)buffer[3] << 24;
			unsigned short total = (length >> 16) ^ (length & 0xFFFF);
			p_length = total;
			bufferpos = 0;
			curp = packet();
		}

		if (bufferpos == 0)
			curp.writebytes(buffer + 4, min(result - 4, p_length + 2));
		else
			curp.writebytes(buffer, min(result, p_length + 2));

		bufferpos += result;

		if (bufferpos >= p_length)
		{
			int processed = p_length + 4;

			packet_h.process(curp);
			p_length = 0;

			if (result > processed)
			{
				char* remaining = buffer + (processed);
				process(remaining, result - processed);
			}
		}
	}

	void session::close()
	{
		active = false;
		closesocket(sock);
		WSACleanup();
	}
}