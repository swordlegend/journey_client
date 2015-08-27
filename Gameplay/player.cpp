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
#include "player.h"

namespace gameplay
{
	player::player(maplechar* ch, inventory inv, int meso, map<int, pair<pair<int, int>, long>> skil, map<int, short> cd, map<int, pair<string, pair<short, string>>> quest, map<int, long> cquest, pair<vector<int>, vector<int>> trock, int cov, map<short, char> mb, map<short, string> area)
	{
		look = ch->copylook();
		stats = ch->copystats();
		invent = inv;
		stats.setmeso(meso);

		recalcstats(true);

		skills = skil;
		cooldowns = cd;
		quests = quest;
		completedquests = cquest;
		trockmaps = trock;
		bookcover = cov;
		bookcards = mb;
		areainfo = area;
		hspeed = 0;
		vspeed = 0;
		state = mst_stand;
		ladrrope = ladderrope();
		ladrrope.vertical = vector2d();
		ladrrope.ladder = false;
		nofriction = false;
		attacking = false;
		fleft = true;
		look.setfleft(true);
		candjump = true;

		for (moveinput i = min_left; i <= min_attack; i = static_cast<moveinput>(i + 1))
		{
			keydown[i] = false;
		}
	}

	maplestats* player::getstats()
	{
		return &stats;
	}

	inventory* player::getinventory()
	{
		return &invent;
	}

	vector2d player::getposition()
	{
		return position;
	}

	void player::updateskill(int id, int lv, int mlv, long expi)
	{
		skills[id] = pair<pair<int, int>, long>(make_pair(lv, mlv), expi);
	}

	void player::setposition(vector2d pos)
	{
		position = pos;
		fx = static_cast<float>(position.x());
		fy = static_cast<float>(position.y());

		hspeed = 0;
		vspeed = 0;

		ground = footholds->getgroundbelow(position);
		if (ground > fy)
		{
			state = mst_fall;
		}
		else if (state == mst_climb)
		{
			state = mst_stand;
		}
	}

	void player::setexpression(char exp)
	{
		look.setexpression(exp);
	}

	void player::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		fx = fx + hspeed;
		fy = fy + vspeed;

		position = vector2d(static_cast<int>(fx), static_cast<int>(fy));
		look.draw(target, parentpos + position);
	}

	movep_info player::update()
	{
		movep_info ret;

		bool anidone = look.update();

		float fspeed = walkSpeed * static_cast<float>(speed) / 100;

		if (state == mst_climb)
		{
			if (keydown[min_up])
			{
				vspeed = -fspeed;
			}
			else if (keydown[min_crouch])
			{
				vspeed = fspeed;
			}
			else
			{
				vspeed = 0;
			}

			float cfriction = 0.01f;

			if (vspeed > 0)
			{
				vspeed -= cfriction;
				vspeed = max(vspeed, 0);
			}
			else if (vspeed < 0)
			{
				vspeed += cfriction;
				vspeed = min(vspeed, 0);
			}

			if (fy + vspeed > ladrrope.vertical.y() || fy + vspeed < ladrrope.vertical.x())
			{
				state = mst_fall;
			}
		}
		else
		{
			if (state != mst_attack && state != mst_prone)
			{
				float maxhspeed = (candjump) ? fspeed * 1.5 : fspeed * 3;

				if (keydown[min_left])
				{
					hspeed = hspeed - (fspeed / 10);
					hspeed = max(hspeed, -maxhspeed);
				}
				else if (hspeed < 0)
				{
					hspeed += 0.13f;
					hspeed = min(hspeed, 0);
				}

				if (keydown[min_right])
				{
					hspeed = hspeed + (fspeed / 10);
					hspeed = min(hspeed, maxhspeed);
				}
				else if (hspeed > 0)
				{
					hspeed -= 0.13f;
					hspeed = max(hspeed, 0);
				}
			}

			if (hspeed != 0 && !nofriction)
			{
				float friction;

				switch (state)
				{
				case mst_attack:
					friction = 0.2f;
					break;
				case mst_prone:
				case mst_stand:
				case mst_climb:
				case mst_walk:
					friction = 0.08f;
					break;
				case mst_fall:
					friction = 0.01f;
					break;
				}

				if (hspeed > 0)
				{
					hspeed -= friction;
					hspeed = max(hspeed, 0);
				}
				else if (hspeed < 0)
				{
					hspeed += friction;
					hspeed = min(hspeed, 0);
				}
			}

			if (hspeed != 0 && vspeed == 0)
			{
				ground = footholds->nextground(hspeed < 0, position);
				if (abs(ground - fy) <= 7)
				{
					fy = ground;
				}
			}
			else if (vspeed != 0)
			{
				ground = footholds->getgroundbelow(position);
			}

			if (ground != fy)
			{
				vspeed = vspeed + gravityAcc;
				vspeed = min(vspeed, fallspeed);

				if (state != mst_attack)
				{
					state = mst_fall;
				}
			}

			if (vspeed != 0)
			{
				if (fy + vspeed * 2 >= ground && ground >= fy)
				{
					vspeed = 0;
					fy = ground;
					candjump = true;

					if (state != mst_attack)
					{
						if (keydown[min_left] || keydown[min_right])
						{
							state = mst_walk;
						}
						else if (keydown[min_crouch])
						{
							state = mst_prone;
						}
						else
						{
							state = mst_stand;
						}
					}
				}
			}
		}

		if (anidone)
		{
			switch (state)
			{
			case mst_attack:
				state = (vspeed == 0) ? mst_stand : mst_fall;
				if (keydown[min_left])
				{
					fleft = true;
				}
				if (keydown[min_right])
				{
					fleft = false;
				}
				break;
			}
		}

		look.setfleft(fleft);

		switch (state)
		{
		case mst_stand:
			if (hspeed == 0)
			{
				look.setstate("stand1");
			}
			else
			{
				look.setstate("walk1");
			}
			break;
		case mst_walk:
			look.setstate("walk1");
			break;
		case mst_fall:
			look.setstate("jump");
			break;
		case mst_prone:
			look.setstate("prone");
			break;
		case mst_climb:
			look.setstate((ladrrope.ladder)? "ladder" : "rope");
			break;
		case mst_attack:
			look.setstate("stabO1");
			break;
		}

		ret.xpps = static_cast<short>(hspeed * 60);
		ret.ypps = static_cast<short>(vspeed * 60);
		ret.xpos = static_cast<short>(fx + hspeed);
		ret.ypos = static_cast<short>(fy + vspeed);
		ret.command = 0;
		ret.duration = 0;
		ret.unk = 0;
		return ret;
	}

	void player::key_left(bool kst)
	{
		if (kst)
		{
			keydown[min_right] = false;

			switch (state)
			{
			case mst_stand:
				fleft = true;
				state = mst_walk;
				break;
			case mst_walk:
				if (!fleft)
				{
					fleft = true;
					hspeed = 0;
				}
				else if (hspeed < 0 && !keydown[min_left])
				{
					//hspeed = hspeed - 5;
				}
				break;
			case mst_fall:
				fleft = true;
				break;
			}
		}
		else
		{
			if (!keydown[min_right] && state == mst_walk)
			{
				state = mst_stand;
			}
		}

		keydown[min_left] = kst;
	}

	void player::key_right(bool kst)
	{
		if (kst)
		{
			keydown[min_left] = false;

			switch (state)
			{
			case mst_stand:
				fleft = false;
				state = mst_walk;
				break;
			case mst_walk:
				if (fleft)
				{
					fleft = false;
					hspeed = 0;
				}
				else if (hspeed > 0 && !keydown[min_left])
				{
					//hspeed = hspeed + 5;
				}
				break;
			case mst_fall:
				fleft = false;
				break;
			}
		}
		else
		{
			if (!keydown[min_left] && state == mst_walk)
			{
				state = mst_stand;
			}
		}

		keydown[min_right] = kst;
	}

	void player::key_jump(bool kst)
	{
		if (!keydown[min_jump] && kst)
		{
			switch (state)
			{
			case mst_stand:
			case mst_walk:
				if (keydown[min_crouch])
				{
					float gbelow = footholds->getgroundbelow(position + vector2d(0, 5));
					if (gbelow > ground && gbelow - ground < 1000)
					{
						setposition(position + vector2d(0, 5));
					}
					else
					{
						vspeed = -jumpSpeed * (static_cast<float>(jump) / 100);
					}
				}
				else
				{
					vspeed = -jumpSpeed * (static_cast<float>(jump) / 100);
				}
				state = mst_fall;
				break;
			case mst_fall:
				if (candjump)
				{
					hspeed = 8 * (static_cast<float>(jump) / 100);
					hspeed *= (fleft) ? -1 : 1;
					vspeed = -jumpSpeed * (static_cast<float>(jump) / 100);
					candjump = false;
				}
				break;
			case mst_prone:
				if (candjump)
				{
					float gbelow = footholds->getgroundbelow(position + vector2d(0, 5));
					if (gbelow > ground && gbelow - ground < 1000)
					{
						setposition(position + vector2d(0, 5));
					}
					else
					{
						vspeed = -jumpSpeed * (static_cast<float>(jump) / 100);
					}
					state = mst_fall;
				}
				break;
			case mst_climb:
				if (keydown[min_left] || keydown[min_right])
				{
					vspeed = -jumpSpeed * (static_cast<float>(jump) / 200);
					state = mst_fall;
					ladrrope.vertical = vector2d();
				}
				break;
			}
		}

		keydown[min_jump] = kst;
	}

	void player::key_down(bool kst)
	{
		if (kst)
		{
			switch (state)
			{
			case mst_stand:
			case mst_walk:
				hspeed = 0;
				state = mst_prone;
				break;
			case mst_climb:
				break;
			}
		}
		else
		{
			switch (state)
			{
			case mst_prone:
				state = mst_stand;
				break;
			}
		}

		keydown[min_crouch] = kst;
	}

	void player::key_up(bool kst)
	{
		keydown[min_up] = kst;
	}

	bool player::attack(int skill)
	{
		if (state != mst_attack && state != mst_climb)
		{
			state = mst_attack;
			return true;
		}
		else
		{
			return false;
		}
	}

	void player::recalcstats(bool param)
	{
		speed = 100;
		jump = 100;

		invent.recalcstats();

		basedamage = static_cast<int>((static_cast<float>(5 * stats.getstat(LEVEL))/100) * invent.gettotal(ES_WATK) * invent.getwepmult());
	}

	void player::setlr(ladderrope lr)
	{
		ladrrope = lr;
		hspeed = 0;
		vspeed = min(vspeed, 0);
		fx = static_cast<float>(lr.x);
		state = mst_climb;
	}

	void player::setfh(footholdtree* fh)
	{
		footholds = fh;
	}
}
