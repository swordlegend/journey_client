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
#include "mapmobs.h"
#include "collision.h"

namespace gameplay
{
	void mapmobs::addmob(int oid, int id, bool control, vector2d pos, char stance, short fh, char effect, bool fadein, char team)
	{
		mobs.add(oid, mob(id, oid, control, pos, stance, fh, effect, fadein, team));
	}

	void mapmobs::sendattack(attackinfo* attack, int basedamage, pair<vector2d, vector2d> range)
	{
		attack->numattacked = 0;
		for (int i = 0; i < mobs.getend(); i++)
		{
			mob* mobit = mobs.getnext(i);

			if (colliding(make_pair(mobit->getposition(), mobit->getdimension()), range) && mobit->isalive())
			{
				mobit->damage(attack, basedamage);
				attack->numattacked++;

				if (attack->numattacked >= attack->maxattacked)
					break;
			}
		}
	}

	void mapmobs::sendmobhp(int oid, char percentage)
	{
		if (mobs.contains(oid))
		{
			mobs.get(oid)->showhp(percentage);
		}
	}

	void mapmobs::movemob(int oid, char movement, bool fleft)
	{
	}

	void mapmobs::killmob(int oid, char animation)
	{
		if (mobs.contains(oid))
		{
			mobs.get(oid)->kill(animation);
		}
	}

	void mapmobs::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		for (int i = 0; i < mobs.getend(); i++)
		{
			mobs.getnext(i)->draw(target, viewpos);
		}
	}

	void mapmobs::update()
	{
		vector<int> toremove;

		for (int i = 0; i < mobs.getend(); i++)
		{
			bool died = mobs.getnext(i)->update();

			if (died)
			{
				toremove.push_back(i);
			}
		}

		if (toremove.size() > 0)
		{
			for (vector<int>::iterator rmvit = toremove.begin(); rmvit != toremove.end(); ++rmvit)
			{
				int oid = *rmvit;
				mobs.remove(oid);
			}
		}
	}

	void mapmobs::clear()
	{
		mobs.clear();
	}
}
