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
#include "npc.h"
#include "mapmobs.h"
#include "reactor.h"
#include "itemdrop.h"

namespace gameplay
{
	enum mmotype : char
	{
		MOT_NPC,
		MOT_MOB,
		MOT_REACTOR,
		MOT_DROP
	};

	class mapobjects
	{
	public:
		mapobjects();
		~mapobjects() {}
		void clear();
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
		void removedrop(int);
		void addnpc(int, npc);
		void addreactor(int, reactor);
		void additemdrop(int, itemdrop);
		mapmobs* getmobs() { return &mobs; }
	private:
		map<int, mmotype> objects;
		map<int, npc> npcs;
		mapmobs mobs;
		map<int, reactor> reactors;
		map<int, itemdrop> drops;
		SRWLOCK objlock;
	};
}

