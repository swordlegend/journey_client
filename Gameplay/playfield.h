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
#include "mapinfo.h"
#include "mapbackgrounds.h"
#include "maplayer.h"
#include "mapobjects.h"
#include "footholdtree.h"
#include "laddersropes.h"
#include "mapportals.h"
//#include "Mp3.h"

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
		void draw(ID2D1HwndRenderTarget*);
		void update();
		void setplayer(player);
		void setfield(int, char);
		void changechannel(char) {}
		void setworldchannel(char wld, char chd) { worldid = wld; channelid = chd; }
		bool moveup(bool);
		void useattack(int);
		player* getplayer() { return &playerchar; }
		account* getaccount() { return &m_account; }
		mapobjects* getmapobjects() { return &map_objects; }
		map<char, world>* getworlds() { return &worlds; }
		vector2d getviewpos() { return view.getposition(); }
		char getchannel() { return channelid; }
	private:
		player playerchar;
		camera view;
		mapinfo map_info;
		mapbackgrounds backgrounds;
		map<char, maplayer> maplayers;
		mapobjects map_objects;
		footholdtree footholds;
		laddersropes landr;
		mapportals portals;
		account m_account;
		gamestep step;
		//Mp3 bgm;
		map<char, world> worlds;
		char worldid;
		char channelid;
	};
}

