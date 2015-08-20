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
#include "mob.h"

namespace gameplay
{
	mob::mob(map<string, animation> t, string n, short l, short s)
	{
		textures = t;
		name = n;
		level = l;
		speed = s;
		state = "stand";
		moved = 0;
		pos = vector2d();
		walls = vector2d();
		hspeed = 0;
		vspeed = 0;
		fx = 0;
		fy = 0;
	}

	void mob::setinfo(int id, vector2d p, vector2d w, char s, short f, char e, bool fd, char t)
	{
		oid = id;
		pos = p;
		walls = w;
		stance = s;
		fh = f;
		effect = e;
		fadein = fd;
		team = t;
		hppercent = 0;
		knockback = 0;
		fx = static_cast<float>(p.x());
		fy = static_cast<float>(p.y());
	}

	void mob::damage(attackinfo* attack, int basedamage)
	{
		vector<int> dmgnumbers;
		long totaldamage = 0;

		for (char i = 0; i < attack->numdamage; i++)
		{
			srand(time(0));
			double rand_d = static_cast<double>(rand());
			int damage = basedamage * ((rand_d / RAND_MAX) + attack->mastery);
			dmgnumbers.push_back(damage);
			totaldamage += damage;
		}

		if (totaldamage > knockback && stance == 0)
		{
			state = "hit1";
			moved = 0;
			hspeed = (attack->direction == 1) ? -0.25 : 0.25;
		}

		attack->mobsdamaged.insert(make_pair(oid, dmgnumbers));
	}

	void mob::showhp(char percent)
	{
		hppercent = percent;
	}

	bool mob::update()
	{
		bool aniend = textures[state].update();

		fx = fx + hspeed;
		fy = fy + vspeed;

		if (state == "hit1")
		{
			moved = moved + abs(hspeed);

			if (moved > 5)
			{
				hspeed = 0;
				state = "stand";
			}
		}
		else if (speed != 0)
		{

		}

		return state == "die1" && aniend;
	}

	void mob::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		pos = vector2d(static_cast<int>(fx), static_cast<int>(fy));

		textures[state].draw(target, parentpos + pos);

		if (hppercent > 0)
		{

		}
	}
}
