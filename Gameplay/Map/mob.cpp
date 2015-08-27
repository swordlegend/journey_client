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
#include "mob.h"
#include "Journey.h"
#include "nxfile.h"

namespace gameplay
{
	mob::mob(int id, int o, bool con, vector2d p, char s, short f, char e, bool fd, char t)
	{
		app.getimgcache()->setmode(ict_map);
		nx::view_file("Mob");

		string fullname;
		string strid = to_string(id);
		char extend = 7 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		fullname.append(strid);
		node mobdata = nx::nodes["Mob"].resolve(fullname + ".img");

		for (node subnode = mobdata.begin(); subnode != mobdata.end(); ++subnode)
		{
			string state = subnode.name();

			if (state == "info")
			{
				level = static_cast<short>(subnode.resolve("level").get_integer());
				speed = 100 + static_cast<short>(subnode.resolve("speed").get_integer());
			}
			else
			{
				textures[state] = animation(subnode);
			}
		}

		nx::unview_file("Mob");
		nx::view_file("String");

		node stringdata = nx::nodes["String"].resolve("Mob.img/" + strid);

		node namenode = stringdata.resolve("name");
		if (namenode.istype(stringnode))
			name = namenode.get_string();
		else
			name = "";

		nx::unview_file("String");
		app.getimgcache()->unlock();

		state = "stand";
		oid = o;
		pos = p;
		control = con;
		stance = s;
		fh = f;
		effect = e;
		fadein = fd;
		team = t;

		hppercent = 0;
		knockback = 0;
		fx = static_cast<float>(p.x());
		fy = static_cast<float>(p.y());
		moved = 0;
		hspeed = 0;
		vspeed = 0;
		fleft = stance == 1;
		alpha = (fadein) ? 0.0f : 1.0f;
		fade = false;
		dead = false;
	}

	void mob::damage(attackinfo* attack, int basedamage)
	{
		if (!dead)
		{
			vector<int> dmgnumbers;
			long totaldamage = 0;

			random_device rd;
			uniform_int_distribution<int> udist(0, 100);
			default_random_engine e1(rd());

			for (char i = 0; i < attack->numdamage; i++)
			{
				int damage = static_cast<int>(static_cast<float>(basedamage)* (min((static_cast<float>(udist(e1)) / 100) + attack->mastery, 1)));
				dmgnumbers.push_back(damage);
				totaldamage += damage;
			}

			if (totaldamage > knockback)
			{
				state = "hit1";
				moved = 0;
				hspeed = (attack->direction == 1) ? -0.1f : 0.1f;
			}

			app.getui()->getbase()->showdamage(dmgnumbers, pos - vector2d(0, textures[state].getdimension(0).y()), false, false);

			attack->mobsdamaged.insert(make_pair(oid, dmgnumbers));
		}
	}

	void mob::kill(char animation)
	{
		switch (animation)
		{
		case 0:
			fade = true;
			alpha = 0.0f;
			break;
		case 1:
			state = "die1";
			break;
		case 2:
			fade = true;
			break;
		}
		dead = true;
	}

	void mob::showhp(char percent)
	{
		hppercent = percent;
	}

	void mob::setmove(char movement, bool fl)
	{
		float fspeed = static_cast<float>(speed) / 100;
		fleft = fl;

		switch (movement)
		{
		case 0:
			state = "stand";
			hspeed = 0;
			break;
		case 1:
			state = "move";
			hspeed = (fleft) ? -fspeed : fspeed;
			break;
		}
	}

	bool mob::update()
	{
		bool aniend = textures[state].update();

		fx = fx + hspeed;
		fy = fy + vspeed;

		if (state == "hit1")
		{
			moved += abs(hspeed);

			if (moved > 5)
			{
				hspeed = 0;
				state = "stand";
			}
			else
			{
				hspeed *= 1.25;
			}
		}
		else if (control)
		{
			float fspeed = static_cast<float>(speed) / 100;

			if (state == "stand")
			{
				if (moved < 100)
				{
					moved += 1;
				}
				else if (aniend)
				{
					moved = 0;
					state = "move";

					random_device rd;
					uniform_int_distribution<int> udist(0, 1);
					default_random_engine e1(rd());

					fleft = udist(e1) == 1;
					hspeed = (fleft) ? -fspeed : fspeed;
				}
			}
			else
			{
				if (moved < 100)
				{
					moved += 1;
				}
				else if (aniend)
				{
					moved = 0;

					random_device rd;
					uniform_int_distribution<int> udist(0, 2);
					default_random_engine e1(rd());
					int result = udist(e1);

					if (result == 2)
					{
						hspeed = 0;
						state = "stand";
					}
					else
					{
						fleft = udist(e1) == 1;
						hspeed = (fleft) ? -fspeed : fspeed;
					}
				}
			}
		}

		if (fade)
		{
			alpha -= 0.05;
			if (alpha < 0.05)
			{
				aniend = true;
			}
		}

		if (fadein)
		{
			alpha += alpha;
			if (alpha > 0.95)
			{
				alpha = 0.1f;
				fadein = false;
			}
		}

		return dead && aniend;
	}

	void mob::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		pos = vector2d(static_cast<int>(fx), static_cast<int>(fy));
		absp = pos + parentpos;

		if (!fleft)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(-1.0f, 1.0f),
				D2D1::Point2F(
				(float)absp.x(),
				(float)absp.y())));
		}

		textures[state].draw(target, absp, alpha);

		if (!fleft)
		{
			target->SetTransform(
				D2D1::Matrix3x2F::Scale(
				D2D1::Size(1.0f, 1.0f),
				D2D1::Point2F(
				(float)absp.x(),
				(float)absp.y())));
		}

		if (hppercent > 1)
		{
			app.getui()->getbase()->drawmobhp(hppercent, absp - vector2d(30, textures[state].getdimension(0).y() + 25));
		}
	}
}
