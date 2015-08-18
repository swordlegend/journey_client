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
#include "packetcreator.h"

namespace net
{
	void packetcreator::init(session* srv)
	{
		server = srv;
	}

	void packetcreator::c_login(string account, string pass)
	{
		packet p = packet(LOGIN);
		p.writestr(account);
		p.writestr(pass);
		server->dispatch(p);
	}

	void packetcreator::pong()
	{
		packet p = packet(PONG);
		server->dispatch(p);
	}

	void packetcreator::serverlrequest()
	{
		packet p = packet(SERVER_REQUEST);
		server->dispatch(p);
	}

	void packetcreator::banme()
	{
		packet p = packet(BANME);
		server->dispatch(p);
	}

	void packetcreator::charlrequest(char world, char channel)
	{
		packet p = packet(CHARL_REQ);
		p.writech(world);
		p.writech(channel + 1);
		server->dispatch(p);
	}

	void packetcreator::deletechar(string pic, int cid)
	{
		packet p = packet(DELETE_CHAR);
		p.writestr(pic);
		p.writeint(cid);
		server->dispatch(p);
	}

	void packetcreator::selectchar(int cid)
	{
		packet p = packet(SELECT_CHAR);
		p.writeint(cid);
		server->dispatch(p);
	}

	void packetcreator::playerlogin(int cid)
	{
		packet p = packet(PLAYER_LOGIN);
		p.writeint(cid);
		server->dispatch(p);
	}

	void packetcreator::changemap(bool died, int targetid, string targetpname, bool usewheel)
	{
		packet p = packet(CHANGEMAP);
		p.writebl(died);
		p.writeint(targetid);
		p.writestr(targetpname);
		p.writebl(usewheel);
		server->dispatch(p);
	}
}