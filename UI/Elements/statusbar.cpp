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
#include "statusbar.h"
#include "Journey.h"

namespace io
{
	statusbar::statusbar(nxprovider* provider, maplestats* pstats)
	{
		provider->setcmode(CM_SYS);
		nl::nx::view_file("UI");

		node mainbar = nl::nx::nodes["UI"].resolve("StatusBar2.img/mainBar");
		node chat = nl::nx::nodes["UI"].resolve("StatusBar2.img/chat");

		sprites.push_back(sprite(provider->loadanimation(mainbar.resolve("backgrnd")), vector2d(0, 0)));
		sprites.push_back(sprite(provider->loadanimation(mainbar.resolve("gaugeBackgrd")), vector2d(0, 0)));
		sprites.push_back(sprite(provider->loadanimation(mainbar.resolve("notice")), vector2d(0, 0)));
		sprites.push_back(sprite(provider->loadanimation(mainbar.resolve("lvBacktrnd")), vector2d(0, 0)));
		sprites.push_back(sprite(provider->loadanimation(mainbar.resolve("lvCover")), vector2d(0, 0)));

		exp = uibar(provider->loadtexture(mainbar.resolve("gauge/exp/0")), provider->loadtexture(mainbar.resolve("gauge/exp/1")), provider->loadtexture(mainbar.resolve("gauge/exp/2")), 308, vector2d(-261, -15));
		hp = uibar(provider->loadtexture(mainbar.resolve("gauge/hp/0")), provider->loadtexture(mainbar.resolve("gauge/hp/1")), provider->loadtexture(mainbar.resolve("gauge/hp/2")), 137, vector2d(-261, -31));
		mp = uibar(provider->loadtexture(mainbar.resolve("gauge/mp/0")), provider->loadtexture(mainbar.resolve("gauge/mp/1")), provider->loadtexture(mainbar.resolve("gauge/mp/2")), 137, vector2d(-90, -31));

		statset = provider->loadcharset(mainbar.resolve("gauge/number"));
		lvset = provider->loadcharset(mainbar.resolve("lvNumber"));

		buttons.insert(make_pair(BT_CHAT, button(provider->loadbutton(mainbar.resolve("BtChat")), 0, 0)));
		buttons.insert(make_pair(BT_CALLGM, button(provider->loadbutton(mainbar.resolve("BtClaim")), 0, 0)));

		buttons.insert(make_pair(BT_CASHSHOP, button(provider->loadbutton(mainbar.resolve("BtCashShop")), 0, 0)));
		buttons.insert(make_pair(BT_TRADE, button(provider->loadbutton(mainbar.resolve("BtMTS")), 0, 0)));
		buttons.insert(make_pair(BT_MENU, button(provider->loadbutton(mainbar.resolve("BtMenu")), 0, 0)));
		buttons.insert(make_pair(BT_SYSOP, button(provider->loadbutton(mainbar.resolve("BtSystem")), 0, 0)));
		//buttons.insert(make_pair(BT_CHANNEL, button(provider->loadbutton(mainbar.resolve("BtChannel")), 0, 0)));

		buttons.insert(make_pair(BT_CHARINFO, button(provider->loadbutton(mainbar.resolve("BtCharacter")), 0, 0)));
		buttons.insert(make_pair(BT_STATS, button(provider->loadbutton(mainbar.resolve("BtStat")), 0, 0)));
		buttons.insert(make_pair(BT_QUEST, button(provider->loadbutton(mainbar.resolve("BtQuest")), 0, 0)));
		buttons.insert(make_pair(BT_INVENTORY, button(provider->loadbutton(mainbar.resolve("BtInven")), 0, 0)));
		buttons.insert(make_pair(BT_EQUIPS, button(provider->loadbutton(mainbar.resolve("BtEquip")), 0, 0)));
		buttons.insert(make_pair(BT_SKILL, button(provider->loadbutton(mainbar.resolve("BtSkill")), 0, 0)));
		//buttons.insert(make_pair(BT_FARM, button(provider->loadbutton(mainbar.resolve("BtFarm")), 0, 0)));
		//buttons.insert(make_pair(BT_KEYMAP, button(provider->loadbutton(mainbar.resolve("BtKeysetting")), 0, 0)));

		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(512, 590);
		dimensions = vector2d(1366, 80);
		active = true;
		visible = true;
		stats = pstats;
	}

	void statusbar::draw(ID2D1HwndRenderTarget* target)
	{
		uielement::draw(target);

		exp.draw(target, position, stats->exp / 10, stats->exp);
		hp.draw(target, position, stats->hp, stats->mhp);
		mp.draw(target, position, stats->mp, stats->mmp);

		string exppercent = to_string(((double)stats->exp) / (((double)stats->exp) / 10));
		statset.draw(target, to_string(stats->exp) + "[" + exppercent.substr(0, exppercent.find('.') + 3) + "%]", cha_right, position + vector2d(47, -13));
		statset.draw(target, "[" + to_string(stats->hp) + "/" + to_string(stats->mhp) + "]", cha_right, position + vector2d(-124, -29));
		statset.draw(target, "[" + to_string(stats->mp) + "/" + to_string(stats->mmp) + "]", cha_right, position + vector2d(47, -29));

		lvset.draw(target, to_string(stats->level), cha_left, position + vector2d(-495, -25));
	}

	void statusbar::update()
	{

	}

	void statusbar::buttonpressed(short id)
	{
		switch (id)
		{
		case BT_MENU:
			quit();
			break;
		}
	}

	pair<vector2d, vector2d> statusbar::bounds()
	{
		return pair<vector2d, vector2d>(position - vector2d(512, 84), dimensions);
	}
}
