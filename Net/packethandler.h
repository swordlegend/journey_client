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
#include "vhandler.h"
#include "nxprovider.h"
#include "ui.h"
#include "maplechar.h"
#include "maplelook.h"
#include "inventory.h"
#include "mapleitem.h"
#include "mapleequip.h"

using namespace io;
using namespace std;
using namespace util;
using namespace gameplay;

namespace net
{
	enum recvpops : short
	{
		LOGIN_RESULT = 0,
		SERVERLIST = 10,
		CHARLIST = 11,
		SERVER_IP = 12,
		DELCHAR_RESPONSE = 15,
		PING = 17,
		WARP_TO_MAP = 18,
		MODIFY_INVENTORY = 29,
		STATS_CHANGED = 31,
		UPDATE_SKILLS = 36,
		SHOW_STATUS_INFO = 39,
		MEMO_RESULT = 41,
		ENABLE_REPORT = 47,
		FORCED_STAT_RESET = 35,
		BUDDY_LIST = 63,
		SERVER_MESSAGE = 68,
		WEEK_EVENT_MESSAGE = 77,
		FAMILY_PRIV_LIST = 100,
		SCRIPT_PROGRESS_MESSAGE = 122,
		SKILL_MACROS = 124,
		CHARINFO = 125,
		CLOCK = 147,
		SPAWN_PET = 168,
		SPAWN_MOB = 236,
		KILL_MOB = 237,
		SPAWN_MOB_C = 238,
		SHOW_MOB_HP = 250,
		SPAWN_NPC = 257,
		DROP_ITEM_FROMOBJECT = 268,
		REMOVE_MAPITEM = 269,
		SPAWN_REACTOR = 279,
		KEYMAP = 335
	};

	class packethandler
	{
	public:
		packethandler();
		~packethandler();
		void process(packet);
	private:
		map<short, unique_ptr<vhandler>> handlers;
		mutex processlock;
	};
}

