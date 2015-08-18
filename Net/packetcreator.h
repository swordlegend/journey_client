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
#include "packet.h"
#include "session.h"

using namespace std;

namespace net
{
	enum sendops : short {
		LOGIN = 1,
		SERVER_REREQ = 4,
		CHARL_REQ = 5,
		STATUS_REQ = 6,
		ACCEPT_TOS = 7,
		SET_GENDER = 8,
		AFTER_LOGIN = 9,
		REG_PIN = 10,
		SERVER_REQUEST = 11,
		SELECT_CHAR = 19,
		PLAYER_LOGIN = 20,
		DELETE_CHAR = 23,
		PONG = 24,
		CHANGEMAP = 38,
		BANME = 4919
	};

	class packetcreator
	{
	private:
		session* server;
	public:
		packetcreator() {}
		~packetcreator() {}
		void init(session*);
		void c_login(string, string);
		void pong();
		void serverlrequest();
		void banme();
		void charlrequest(char, char);
		void deletechar(string, int);
		void selectchar(int);
		void playerlogin(int);
		void changemap(bool, int, string, bool);
	};
}

