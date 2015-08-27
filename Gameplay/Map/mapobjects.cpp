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
#include "mapobjects.h"
#include "collision.h"

namespace gameplay
{
	mapobjects::mapobjects()
	{
		objlock = SRWLOCK_INIT;
	}

	void mapobjects::removedrop(int oid)
	{
		AcquireSRWLockShared(&objlock);
		drops[oid].expire();
		ReleaseSRWLockShared(&objlock);
	}

	void mapobjects::addnpc(int oid, npc toadd)
	{
		AcquireSRWLockExclusive(&objlock);
		objects[oid] = MOT_NPC;
		npcs[oid] = toadd;
		ReleaseSRWLockExclusive(&objlock);
	}

	void mapobjects::addreactor(int oid, reactor toadd)
	{
		AcquireSRWLockExclusive(&objlock);
		objects[oid] = MOT_REACTOR;
		reactors[oid] = toadd;
		ReleaseSRWLockExclusive(&objlock);
	}

	void mapobjects::additemdrop(int oid, itemdrop toadd)
	{
		AcquireSRWLockExclusive(&objlock);
		objects[oid] = MOT_DROP;
		drops[oid] = toadd;
		ReleaseSRWLockExclusive(&objlock);
	}

	void mapobjects::clear()
	{
		AcquireSRWLockExclusive(&objlock);
		objects.clear();
		npcs.clear();
		mobs.clear();
		reactors.clear();
		drops.clear();
		ReleaseSRWLockExclusive(&objlock);
	}

	void mapobjects::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		mobs.draw(target, viewpos);

		if (TryAcquireSRWLockShared(&objlock))
		{
			for (map<int, npc>::iterator npcit = npcs.begin(); npcit != npcs.end(); ++npcit)
			{
				npcit->second.draw(target, viewpos);
			}
			for (map<int, reactor>::iterator reacit = reactors.begin(); reacit != reactors.end(); ++reacit)
			{
				//reacit->second.draw(target, viewpos);
			}
			for (map<int, itemdrop>::iterator drpit = drops.begin(); drpit != drops.end(); ++drpit)
			{
				drpit->second.draw(target, viewpos);
			}

			ReleaseSRWLockShared(&objlock);
		}
	}

	void mapobjects::update()
	{
		vector<int> toremove;

		mobs.update();

		if (TryAcquireSRWLockShared(&objlock))
		{
			for (map<int, npc>::iterator npcit = npcs.begin(); npcit != npcs.end(); ++npcit)
			{
				npcit->second.update();
			}
			for (map<int, reactor>::iterator reacit = reactors.begin(); reacit != reactors.end(); ++reacit)
			{
				//reacit->second.draw(target, viewpos);
			}

			for (map<int, itemdrop>::iterator drpit = drops.begin(); drpit != drops.end(); ++drpit)
			{
				bool removed = drpit->second.update();

				if (removed)
				{
					toremove.push_back(drpit->first);
				}
			}

			ReleaseSRWLockShared(&objlock);
		}

		if (toremove.size() > 0)
		{
			AcquireSRWLockExclusive(&objlock);
			for (vector<int>::iterator rmvit = toremove.begin(); rmvit != toremove.end(); ++rmvit)
			{
				int oid = *rmvit;
				mmotype type = objects[oid];

				switch (type)
				{
				case MOT_REACTOR:
					reactors.erase(oid);
					break;
				case MOT_DROP:
					drops.erase(oid);
					break;
				}
			}
			ReleaseSRWLockExclusive(&objlock);
		}
	}
}
