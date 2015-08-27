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
#include "baseinterface.h"
#include "Journey.h"

namespace io
{
	void baseinterface::init()
	{
		app.getimgcache()->setmode(ict_sys);
		nx::view_file("UI");

		node nametagnode = nx::nodes["UI"]["Login.img"]["CharSelect"]["nameTag"];
		vector<texture> ntbg;
		ntbg.push_back(texture(nametagnode.resolve("0/0")));
		ntbg.push_back(texture(nametagnode.resolve("0/1")));
		ntbg.push_back(texture(nametagnode.resolve("0/2")));
		npctag = nametag(app.getfonts()->getfont(DWF_CENTER), txc_yellow, ntbg, "", vector2d());

		node hpbar = nx::nodes["UI"]["UIWindow2.img"]["EnergyBar"];
		mobhpbar["hpbarfront"] = texture(hpbar["w"]);
		mobhpbar["hpbarmid"] = texture(hpbar["c"]);
		mobhpbar["hpbarend"] = texture(hpbar["e"]);
		mobhpbar["hpbarfill0"] = texture(hpbar["Gage"]["1"]["0"]);
		mobhpbar["hpbarfill1"] = texture(hpbar["Gage"]["1"]["1"]);

		nx::unview_file("UI");
		nx::view_file("Effect");

		dmgset = charset(nx::nodes["Effect"]["BasicEff.img"]["NoRed0"]);
		critset = charset(nx::nodes["Effect"]["BasicEff.img"]["NoCri0"]);
		playerdmgset = charset(nx::nodes["Effect"]["BasicEff.img"]["NoViolet0"]);

		nx::unview_file("Effect");
		app.getimgcache()->unlock();

		infotextw = textlabel(app.getfonts()->getfont(dwf_small_r), txc_white, "");
		infotexty = textlabel(app.getfonts()->getfont(dwf_small_r), txc_yellow, "");

		position = vector2d();
		dimensions = vector2d();
		active = true;
		visible = true;
		dmglock = SRWLOCK_INIT;
	}

	void baseinterface::showdamage(vector<int> num, vector2d pos, bool top, bool cri)
	{
		AcquireSRWLockExclusive(&dmglock);

		dmgeffect toadd;
		toadd.numbers = num;
		toadd.position = pos;
		toadd.fy = static_cast<float>(pos.y());
		toadd.visible = 1;
		toadd.alpha = 1.0f;
		toadd.destination = pos - vector2d(0, 30 * num.size());
		toadd.toplayer = top;
		toadd.crit = cri;
		dmgeffects.push_back(toadd);

		ReleaseSRWLockExclusive(&dmglock);
	}

	void baseinterface::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		if (TryAcquireSRWLockShared(&dmglock))
		{
			for (vector<dmgeffect>::iterator dmgit = dmgeffects.begin(); dmgit != dmgeffects.end(); ++dmgit)
			{
				char numv = dmgit->visible;
				dmgit->position = vector2d(dmgit->position.x(), static_cast<int>(dmgit->fy));

				if (dmgit->toplayer)
				{
					for (char i = 0; i < numv; i++)
					{
						playerdmgset.draw(to_string(dmgit->numbers[i]), 22, cha_center, dmgit->position + vector2d(0, 30 * i) + viewpos, dmgit->alpha);
					}
				}
				else
				{
					if (dmgit->crit)
					{
						for (char i = 0; i < numv; i++)
						{
							critset.draw(to_string(dmgit->numbers[i]), 24, cha_center, dmgit->position + vector2d(0, 30 * i) + viewpos, dmgit->alpha);
						}
					}
					else
					{
						for (char i = 0; i < numv; i++)
						{
							dmgset.draw(to_string(dmgit->numbers[i]), 22, cha_center, dmgit->position + vector2d(0, 30 * i) + viewpos, dmgit->alpha);
						}
					}
				}
			}
			ReleaseSRWLockShared(&dmglock);
		}

		vector2d infotop = vector2d(375, 510 - (statusinfos.size() * 16));
		for (char i = 0; i < statusinfos.size(); i++)
		{
			statusinfo sfit = statusinfos[i];
			if (sfit.white)
			{
				infotextw.settext(sfit.text);
				infotextw.setalpha(sfit.alpha);
				infotextw.draw(target, infotop + vector2d(0, 16 * i));
			}
			else
			{
				infotexty.settext(sfit.text);
				infotexty.setalpha(sfit.alpha);
				infotexty.draw(target, infotop + vector2d(0, 16 * i));
			}
		}
	}

	void baseinterface::update()
	{
		char toremove = 0;
		char inforemove = 0;

		if (TryAcquireSRWLockShared(&dmglock))
		{
			for (vector<dmgeffect>::iterator dmgit = dmgeffects.begin(); dmgit != dmgeffects.end(); ++dmgit)
			{
				dmgit->fy -= 0.5;
				if (dmgit->position.y() - dmgit->destination.y() < 30 * dmgit->numbers.size() - 15)
				{
					dmgit->alpha = static_cast<float>((dmgit->position.y() - dmgit->destination.y())) / (30 * dmgit->numbers.size() - 15);
				}
				dmgit->visible = max(dmgit->numbers.size() - (dmgit->position.y() - dmgit->destination.y()) / 30, 1);

				if (dmgit->position.y() <= dmgit->destination.y())
				{
					toremove++;
				}
			}
			ReleaseSRWLockShared(&dmglock);
		}

		for (vector<statusinfo>::iterator sfit = statusinfos.begin(); sfit != statusinfos.end(); ++sfit)
		{
			sfit->alpha -= 0.005;
			if (sfit->alpha < 0.005)
			{
				inforemove++;
			}
		}

		AcquireSRWLockExclusive(&dmglock);
		for (char i = 0; i < toremove; i++)
		{
			dmgeffects.erase(dmgeffects.begin());
		}
		ReleaseSRWLockExclusive(&dmglock);

		for (char i = 0; i < inforemove; i++)
		{
			statusinfos.erase(statusinfos.begin());
		}
	}

	void baseinterface::drawmobhp(char percent, vector2d pos)
	{
		mobhpbar["hpbarfront"].draw(pos);
		mobhpbar["hpbarmid"].draw(pos + vector2d(5, 0), vector2d(50, 0));
		mobhpbar["hpbarend"].draw(pos + vector2d(55, 0));
		mobhpbar["hpbarfill1"].draw(pos + vector2d(2, 2), vector2d(percent / 4, 0));
	}

	void baseinterface::addstatusinfo(bool white, string text)
	{
		statusinfo info;
		info.white = white;
		info.text = text;
		info.alpha = 1.0f;
		statusinfos.push_back(info);
	}
}
