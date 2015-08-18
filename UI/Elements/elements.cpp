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
#include "elements.h"
#include "Journey.h"

using namespace data;

namespace io
{
	login::login(nxprovider* provider)
	{
		provider->setcmode(CM_LOGIN);
		nl::nx::view_file("UI");

		node title = nl::nx::nodes["UI"].resolve("Login.img/Title/");
		node common = nl::nx::nodes["UI"].resolve("Login.img/Common/");

		sprites.push_back(sprite(provider->loadanimation(title.resolve("11")), vector2d(410, 300)));
		sprites.push_back(sprite(provider->loadanimation(title.resolve("35")), vector2d(410, 260)));
		sprites.push_back(sprite(provider->loadanimation(title.resolve("Logo")), vector2d(410, 130)));
		sprites.push_back(sprite(provider->loadanimation(title.resolve("signboard")), vector2d(410, 300)));
		sprites.push_back(sprite(provider->loadanimation(common.resolve("frame")), vector2d(400, 290)));

		buttons[BT_LOGIN] = button(provider->loadbutton(title.resolve("BtLogin")), 475, 248);
		buttons[BT_NEW] = button(provider->loadbutton(title.resolve("BtNew")), 309, 320);
		buttons[BT_HOMEPAGE] = button(provider->loadbutton(title.resolve("BtHomePage")), 382, 320);
		buttons[BT_PWDLOST] = button(provider->loadbutton(title.resolve("BtPasswdLost")), 470, 300);
		buttons[BT_QUIT] = button(provider->loadbutton(title.resolve("BtQuit")), 455, 320);
		buttons[BT_LOGINLOST] = button(provider->loadbutton(title.resolve("BtLoginIDLost")), 395, 300);
		buttons[BT_SAVEID] = button(provider->loadbutton(title.resolve("BtLoginIDSave")), 325, 300);

		/*sprite check = provider->loadsprite(title.resolve("check"));
		check.setposition(vector2d(313, 304));
		check.setframe((game.getsettings()->accsaved()) ? 1 : 0);
		check.setflag(SF_NO_ANI);
		sprites.push_back(check);*/

		string defaultacc = config.getdefaultacc();
		textfields[TXT_ACC] = textfield(provider->getfont(DWF_LEFT), TXC_WHITE, defaultacc, vector2d(315, 249), 12);
		textfields[TXT_ACC].setbg(provider->loadtexture(title.resolve("ID")), -5, 0);

		textfields[TXT_PASS] = textfield(provider->getfont(DWF_LEFT), TXC_WHITE, "", vector2d(315, 275), 12);
		textfields[TXT_PASS].setbg(provider->loadtexture(title.resolve("PW")), -5, 0);

		if (defaultacc == "")
		{
			textfields[TXT_ACC].setstate("active");
			app.getui()->settextfield(&textfields[0]);
		}
		else
		{
			textfields[TXT_PASS].setstate("active");
			app.getui()->settextfield(&textfields[1]);
		}

		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(800, 600);
		active = true;
		visible = true;
	}

	void login::draw(ID2D1HwndRenderTarget* target)
	{
		uielement::draw(target);

		for (std::map<short, textfield>::iterator ittxt = textfields.begin(); ittxt != textfields.end(); ittxt++)
		{
			ittxt->second.draw(target, position);
		}
	}

	void login::update()
	{
		uielement::update();

		for (std::map<short, textfield>::iterator ittxt = textfields.begin(); ittxt != textfields.end(); ittxt++)
		{
			ittxt->second.update();
		}
	}

	void login::buttonpressed(short id)
	{
		switch (id)
		{
		case BT_LOGIN:
			app.getui()->disableactions();
			app.getui()->add(UI_LOGINWAIT);
			packet_c.c_login(textfields[TXT_ACC].text(), textfields[TXT_PASS].text());
			return;
		case BT_QUIT:
			quit();
			return;
		case BT_SAVEID:
			config.save(3, textfields[TXT_ACC].text());
			return;
		}
	}


	loginwait::loginwait(nxprovider* provider)
	{
		provider->setcmode(CM_LOGIN);
		nl::nx::view_file("UI");
		
		node loading = nl::nx::nodes["UI"].resolve("Login.img/Notice/Loading/");

		sprites.push_back(sprite(provider->loadanimation(loading.resolve("backgrnd")), vector2d()));
		sprites.push_back(sprite(provider->loadanimation(loading.resolve("circle")), vector2d(125, 72)));

		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(292, 200);
		dimensions = vector2d(282, 144);
		active = true;
		visible = true;
	}


	loginnotice::loginnotice(nxprovider* provider, char param)
	{
		provider->setcmode(CM_LOGIN);
		nl::nx::view_file("UI");

		node notice = nl::nx::nodes["UI"].resolve("Login.img/Notice/");

		string back;
		switch (param)
		{
		case 16:
			back = "backgrnd/1";
			buttons[BT_LOGINBANOK] = button(provider->loadbutton(notice.resolve("BtYes")), 100, 100);
			break;
		case 93:
			back = "backgrnd/1";
			buttons[BT_LOGINNOTICEOK] = button(provider->loadbutton(notice.resolve("BtYes")), 100, 100);
			break;
		default:
			back = "backgrnd/0";
			buttons[BT_LOGINNOTICEOK] = button(provider->loadbutton(notice.resolve("BtYes")), 100, 100);
		}

		sprites.push_back(sprite(provider->loadanimation(notice.resolve(back)), vector2d()));
		sprites.push_back(sprite(provider->loadanimation(notice.resolve("text/" + to_string(param))), vector2d(17, 13)));
		
		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(292, 200);
		dimensions = vector2d(362, 219);
		type = param;
		active = true;
		visible = true;
	}

	void loginnotice::buttonpressed(short id)
	{
		switch (id)
		{
		case BT_LOGINBANOK:
			quit();
			return;
		case BT_LOGINNOTICEOK:
			app.getui()->getelement(UI_LOGIN)->setbutton(BT_LOGIN, "normal");
			active = false;
			return;
		}
	}


	loginworld::loginworld(nxprovider* provider, char num, char* chloads)
	{
		provider->clearcache(CM_LOGIN);
		provider->setcmode(CM_LOGIN);
		nl::nx::view_file("UI");
		nl::nx::view_file("Back");
		node back = nl::nx::nodes["Back"].resolve("Back/login.img/back/");
		node login = nl::nx::nodes["UI"].resolve("Login.img/");
		node worlds = login.resolve("WorldSelect/BtWorld/release/");

		sprites.push_back(sprite(provider->loadanimation(back.resolve("11")), vector2d(370, 300)));
		sprites.push_back(sprite(provider->loadanimation(worlds.resolve("layer:bg")), vector2d(650, 45)));
		sprites.push_back(sprite(provider->loadanimation(login.resolve("Common/frame")), vector2d(400, 290)));

		buttons.insert(make_pair(BT_WORLDSEL0, button(provider->loadbutton(worlds.resolve("button:15")), 650, 20)));

		sprites.push_back(sprite(provider->loadanimation(worlds.resolve("button:16/normal")), vector2d(650, 74)));

		for (char i = 14; i >= 0; i--)
		{
			sprites.push_back(sprite(provider->loadanimation(worlds.resolve("button:" + to_string(i) + "/normal")), vector2d(650, 47)));
		}

		node channels = login.resolve("WorldSelect/BtChannel/");

		sprites.push_back(sprite(provider->loadanimation(channels.resolve("layer:bg")), vector2d(200, 170)));
		sprites.push_back(sprite(provider->loadanimation(channels.resolve("release/layer:15")), vector2d(200, 170)));

		for (char i = 0; i < num; i++)
		{
			button chi = button(
				provider->loadtexture(channels.resolve("button:" + to_string(i) + "/normal/0")),
				provider->loadtexture(channels.resolve("button:" + to_string(i) + "/keyFocused/0")),
				200, 170);
			if (i == 0)
				chi.setstate("pressed");
			buttons.insert(make_pair(BT_CHANNELSEL0 + i, chi));
		}

		buttons.insert(make_pair(BT_GOWORLD, button(provider->loadbutton(channels.resolve("button:GoWorld")), 200, 170)));

		nl::nx::unview_file("Back");
		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(800, 600);
		active = true;
		visible = true;
		app.getui()->getfield()->worldid = 0;
		app.getui()->getfield()->channelid = 0;
	}

	void loginworld::buttonpressed(short id)
	{
		if (id >= BT_WORLDSEL0 && id <= BT_WORLDSEL19)
		{
			app.getui()->getfield()->worldid = id - 9;
			for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
			{
				if (itbt->first != id && itbt->first >= BT_WORLDSEL0 && itbt->first <= BT_WORLDSEL19 && itbt->second.getstate() == "pressed")
					itbt->second.setstate("normal");
			}
			return;
		}
		else if (id >= BT_CHANNELSEL0 && id <= BT_CHANNELSEL16)
		{
			app.getui()->getfield()->channelid = id - BT_CHANNELSEL0;
			for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
			{
				if (itbt->first != id && itbt->first >= BT_CHANNELSEL0 && itbt->first <= BT_CHANNELSEL16 && itbt->second.getstate() == "pressed")
					itbt->second.setstate("normal");
			}
			return;
		}
		else if (id == BT_GOWORLD)
		{
			app.getui()->disableactions();
			packet_c.charlrequest(app.getui()->getfield()->worldid, app.getui()->getfield()->channelid);
		}
	}


	logincharsel::logincharsel(nxprovider* provider, char cslots, vector<maplechar>* chars)
	{
		provider->clearcache(CM_LOGIN);
		provider->setcmode(CM_LOGIN);
		nl::nx::view_file("UI");
		
		node login = nl::nx::nodes["UI"].resolve("Login.img");
		node charsel = login.resolve("CharSelect");

		sprites.push_back(sprite(provider->loadanimation(login.resolve("Title/worldsel")), vector2d()));
		sprites.push_back(sprite(provider->loadanimation(login.resolve("Common/frame")), vector2d(400, 290)));
		sprites.push_back(sprite(provider->loadanimation(login.resolve("Common/selectWorld")), vector2d(580, 42)));
		sprites.push_back(sprite(provider->loadanimation(charsel.resolve("selectedWorld/icon/15")), vector2d(580, 42)));
		sprites.push_back(sprite(provider->loadanimation(charsel.resolve("selectedWorld/name/15")), vector2d(580, 42)));
		sprites.push_back(sprite(provider->loadanimation(charsel.resolve("selectedWorld/ch/" + to_string(app.getui()->getfield()->channelid))), vector2d(580, 42)));
		sprites.push_back(sprite(provider->loadanimation(charsel.resolve("charInfo")), vector2d(662, 355)));

		buttons.insert(make_pair(BT_ARBEIT, button(provider->loadbutton(charsel.resolve("arbeit")), 580, 115)));
		buttons.insert(make_pair(BT_CHARCARD, button(provider->loadbutton(charsel.resolve("characterCard")), 665, 115)));

		buttons.insert(make_pair(BT_NEWCHAR, button(provider->loadbutton(charsel.resolve("BtNew")), 200, 495)));
		buttons.insert(make_pair(BT_DELCHAR, button(provider->loadbutton(charsel.resolve("BtDelete")), 320, 495)));

		buttons.insert(make_pair(BT_SELCHAR, button(provider->loadbutton(charsel.resolve("BtSelect")), 586, 427)));

		for (char i = chars->size(); i < cslots; i++)
		{
			sprites.push_back(sprite(provider->loadanimation(charsel.resolve("buyCharacter")), vector2d(130 + (120 * (i % 4)), 250 + (200 * (i > 3)))));
		}

		for (char i = 0; i < chars->size(); i++)
		{
			nametag charname = nametag(provider->getfont(DWF_CENTER), TXC_WHITE, provider->loadnametag(charsel.resolve("nameTag")), chars->at(i).getstats()->name, vector2d(55 + (120 * (i % 4)), 250 + (200 * (i > 3))), (i == 0));
			nametags.push_back(charname);
			buttons.insert(make_pair(BT_CHAR0 + i, button(105 + (120 * (i % 4)), 170 + (200 * (i > 3)), 50, 80)));
			buttons[BT_CHAR0].setstate("pressed");
		}

		provider->unlock();
		provider->setcmode(CM_SYS);

		lvset = provider->loadcharset(charsel.resolve("lv"));
		statset = provider->loadcharset(nl::nx::nodes["UI"].resolve("StatusBar2.img/mainBar/gauge/number"));

		nl::nx::unview_file("UI");
		nl::nx::view_file("Character");

		for (char i = 0; i < chars->size(); i++)
		{
			maplelook* plook = chars->at(i).getlook();
			provider->loadcharlook(plook);
			maplelook look = chars->at(i).copylook();
			look.setposition(vector2d(130 + (120 * (i % 4)), 250 + (200 * (i > 3))));
			look.setfleft(false);
			if (i == 0)
			{
				look.setstate("walk1");
				selected = i;
			}
			looks.push_back(look);
			stats.push_back(chars->at(i).getstats());
		}

		nl::nx::unview_file("Character");
		provider->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(800, 600);
		active = true;
		visible = true;
	}

	void logincharsel::draw(ID2D1HwndRenderTarget* target)
	{
		uielement::draw(target);

		for (vector<maplelook>::iterator itspr = looks.begin(); itspr != looks.end(); itspr++)
		{
			itspr->draw(target, position);
		}
		for (vector<nametag>::iterator itspr = nametags.begin(); itspr != nametags.end(); itspr++)
		{
			itspr->draw(target, position);
		}

		maplestats* info = stats[selected];

		lvset.draw(target, 'l', vector2d(648, 262));
		lvset.draw(target, to_string(info->level), 12, cha_left, vector2d(655, 262));

		statset.draw(target, to_string(info->str), cha_right, vector2d(655, 385));
		statset.draw(target, to_string(info->dex), cha_right, vector2d(655, 407));
		statset.draw(target, to_string(info->int_), cha_right, vector2d(732, 385));
		statset.draw(target, to_string(info->luk), cha_right, vector2d(732, 407));
	}

	void logincharsel::update()
	{
		uielement::update();

		for (vector<maplelook>::iterator itspr = looks.begin(); itspr != looks.end(); itspr++)
		{
			itspr->update();
		}
	}

	void logincharsel::buttonpressed(short id)
	{
		if (id >= BT_CHAR0 && id <= BT_CHAR7)
		{
			selected = id - BT_CHAR0;
			looks[selected].setstate("walk1");
			for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
			{
				if (itbt->first != id && itbt->first >= BT_CHAR0 && itbt->first <= BT_CHAR7 && itbt->second.getstate() == "pressed")
				{
					itbt->second.setstate("normal");
					looks[itbt->first - BT_CHAR0].setstate("stand1");
				}
			}
			return;
		}

		int cid = stats[selected]->id;

		switch (id)
		{
		case BT_DELCHAR:
			packet_c.deletechar("rando", cid);
			break;
		case BT_SELCHAR:
			app.getui()->disableactions();
			app.getui()->getfield()->getaccount()->selectchar(cid);
			packet_c.selectchar(cid);
			break;
		}
	}


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
		statset.draw(target, "[" + to_string(stats->mp) +"/" + to_string(stats->mmp) + "]", cha_right, position + vector2d(47, -29));

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
