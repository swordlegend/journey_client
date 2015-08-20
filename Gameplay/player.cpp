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
	player::player() {}
	player::~player() {}

	player::player(maplechar* ch, inventory inv, map<int, pair<pair<int, int>, long>> skil, map<int, short> cd, map<int, pair<string, pair<short, string>>> quest, map<int, long> cquest, pair<vector<int>, vector<int>> trock, int cov, map<short, char> mb, map<short, string> area)
	{
		look = ch->copylook();
		stats = ch->copystats();
		invent = inv;

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
		movestate = MC_NONE;
		standing = true;
		nofriction = false;
		fleft = true;
		look.setfleft(true);
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
		standing = position.y() >= ground;

		if (standing)
			look.setstate("stand1");
		else
			look.setstate("jump");
	}

	void player::setexpression(char exp)
	{
		look.setexpression(exp);
	}

	void player::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		if (fleft)
		{
			fx = fx - hspeed;
		}
		else
		{
			fx = fx + hspeed;
		}

		fy = fy + vspeed;

		position = vector2d((int)fx, (int)fy);
		look.draw(target, parentpos + position);
	}

	void player::update()
	{
		look.update();

		float fspeed = ((float)speed / 100) * walkSpeed;
		float fjump = ((float)jump / 100);

		if (movestate % MC_JUMP != 0)
		{
			hspeed = hspeed + (fspeed / 10);
			if (hspeed > fspeed * 1.5)
				hspeed = static_cast<float>(fspeed * 1.5);
		}

		if (!nofriction)
		{
			hspeed -= (standing)? 0.08 : 0.05;
			if (hspeed < 0)
				hspeed = 0;
		}

		
		if (vspeed > 0)
		{
			ground = footholds->getgroundbelow(position);
		}
		else if (hspeed > 0)
		{
			ground = footholds->nextground(fleft, position);
			standing = position.y() >= ground;
		}

		if (!standing)
		{
			vspeed = vspeed + gravityAcc;
			if (vspeed > fallspeed)
				vspeed = fallspeed;

			movestate = movestate | MC_JUMP;
			look.setstate("jump");

			if (vspeed > 0)
			{
				if (fy + vspeed > ground)
				{
					vspeed = 0;
					fy = ground;
					standing = true;

					movestate = movestate ^ MC_JUMP;

					if (hspeed != 0)
					{
						look.setstate("walk1");
					}
					else
					{
						look.setstate("stand1");
					}
				}
			}
		}
		else
		{
			if (hspeed != 0 && vspeed == 0 && look.getstate() != "walk1")
				look.setstate("walk1");
			else if (hspeed == 0 && vspeed == 0 && look.getstate() == "walk1")
				look.setstate("stand1");
		}
	}

	void player::crouch(bool keydown)
	{
		if (standing)
		{
			if (keydown)
			{
				hspeed = 0;
				look.setstate("prone");
				movestate = MC_CROUCH;
			}
			else if (movestate == MC_CROUCH)
			{
				look.setstate("stand1");
				movestate = movestate ^ MC_CROUCH;
			}
		}
	}

	void player::move(movecode type, bool keydown)
	{
		short newstate = MC_NONE;

		if (keydown)
		{
			newstate = movestate | type;

			bool newleft = fleft;
			switch (newstate ^ movestate)
			{
			case MC_LEFT:
				newleft = true;
				if ((movestate & MC_RIGHT) != MC_NONE)
					newstate = newstate ^ MC_RIGHT;
				break;
			case MC_RIGHT:
				newleft = false;
				if ((movestate & MC_LEFT) != MC_NONE)
					newstate = newstate ^ MC_LEFT;
				break;
			case MC_JUMP:
				if (standing)
				{
					standing = false;
					look.setstate("jump");
					if (movestate == MC_CROUCH)
					{
						float gbelow = footholds->getgroundbelow(position + vector2d(0, 5));
						if (gbelow > ground && gbelow - ground < 1000)
						{
							setposition(position + vector2d(0, 5));
							newstate = MC_JUMP;
						}
						else
						{
							vspeed = -(jumpSpeed * ((float)jump) / 100);
						}
					}
					else
					{
						vspeed = -(jumpSpeed * ((float)jump) / 100);
					}
				}
				break;
			}

			if (newleft != fleft)
			{
				fleft = newleft;
				look.setfleft(newleft);
				hspeed = 0;
			}
		}
		else
		{
			if ((movestate & type) != MC_NONE)
				newstate = movestate ^ type;
		}
		movestate = newstate;
	}

	void player::recalcstats(bool param)
	{
		speed = 100;
		jump = 100;

		invent.recalcstats();

		stats.basedamage = static_cast<int>((static_cast<float>(5 * stats.level)/100) * invent.gettotal(ES_WATK) * invent.getwepmult());
	}

	void player::setfh(footholdtree* fh)
	{
		footholds = fh;
	}
}
