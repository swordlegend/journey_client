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
#include "player.h"
#include "world.h"
#include "account.h"
#include "camera.h"
#include "settings.h"
#include "maplemap.h"

namespace gameplay
{
	enum gamestep : char
	{
		GST_LOGIN,
		GST_TRANSITION,
		GST_GAME,
		GST_CASHSHOP
	};

	class playfield
	{
	public:
		playfield() {}
		~playfield() {}
		char worldid;
		char channelid;
		void draw(ID2D1HwndRenderTarget*);
		void update();
		void setplayer(player);
		void changechannel(char) {}
		void changemap(maplemap, char);
		bool moveup();
		player* getplayer() { return &playerchar; }
		account* getaccount() { return &m_account; }
		maplemap* getmap() { return &m_map; }
		map<char, world>* getworlds() { return &worlds; }
	private:
		player playerchar;
		camera view;
		account m_account;
		gamestep step;
		maplemap m_map;
		map<char, world> worlds;
		/*struct mapinfo mapinfo;
		struct mapsprites mapsprites;
		struct mapobjects mapobjects;*/
	};
}

