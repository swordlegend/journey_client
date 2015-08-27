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

	void packetcreator::moveplayer(movep_info move)
	{
		packet p = packet(MOVE_PLAYER);
		p.writech(move.command);
		p.writesh(move.xpos);
		p.writesh(move.ypos);
		p.writesh(move.xpps);
		p.writesh(move.ypps);
		p.writesh(move.unk);
		p.writech(move.newstate);
		p.writesh(move.duration);
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

	void packetcreator::close_attack(attackinfo attack)
	{
		packet p = packet(CLOSE_ATTACK);
		p.writech(0);

		char numattdmg = 0;
		numattdmg = attack.numattacked;
		numattdmg = numattdmg << 4;
		numattdmg = numattdmg | attack.numdamage;
		p.writech(numattdmg);

		p.writeint(attack.skill);
		p.writeint(attack.charge);
		p.writelg(0);
		p.writech(attack.display);
		p.writech(attack.direction);
		p.writech(attack.stance);
		p.writech(0);
		p.writech(attack.speed);
		p.writeint(0);

		for (map<int, vector<int>>::iterator attit = attack.mobsdamaged.begin(); attit != attack.mobsdamaged.end(); ++attit)
		{
			p.writeint(attit->first);
			p.writelg(0);
			p.writeint(0);
			p.writeint(0);
			p.writesh(0);

			for (vector<int>::iterator dmgit = attit->second.begin(); dmgit != attit->second.end(); ++dmgit)
			{
				p.writeint(*dmgit);
			}

			if (attack.skill != 5221004) {
				p.writeint(0);
			}
		}

		server->dispatch(p);
	}
}