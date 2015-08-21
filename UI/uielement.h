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
#include "stdfax.h"
#include "button.h"
#include "textfield.h"
#include "collision.h"
#include "maplelook.h"
#include "nametag.h"
#include "sprite.h"

using namespace std;
using namespace util;

namespace io
{
	enum uielements : char
	{
		UI_LOGIN,
		UI_LOGINNOTICE,
		UI_LOGINWAIT,
		UI_WORLDSELECT,
		UI_CHARSEL,
		UI_CHARDELCONFIRM,
		UI_STATUSBAR,
		UI_QUESTS,
		UI_INVENTORY,
		UI_EQUIPS
	};

	enum buttonid : short
	{
		BT_LOGIN = 0,
		BT_NEW = 1,
		BT_HOMEPAGE = 2,
		BT_PWDLOST = 3,
		BT_QUIT = 4,
		BT_LOGINBANOK = 5,
		BT_LOGINNOTICEOK = 6,
		BT_LOGINLOST = 7,
		BT_SAVEID = 8,
		BT_WORLDSEL0 = 9,
		BT_WORLDSEL19 = 29,
		BT_CHANNELSEL0 = 30,
		BT_CHANNELSEL16 = 47,
		BT_GOWORLD = 48,
		BT_ARBEIT = 49,
		BT_CHARCARD = 50,
		BT_DELCHAR = 51,
		BT_NEWCHAR = 52,
		BT_SELCHAR = 53,
		BT_DELCHARCA = 54,
		BT_DELCHARCON = 55,
		BT_CHAR0 = 56,
		BT_CHAR7 = 63,
		BT_CASHSHOP = 64,
		BT_TRADE = 65,
		BT_MENU = 66,
		BT_SYSOP = 67,
		BT_CHANNEL = 68,
		BT_CHARINFO = 69,
		BT_STATS = 70,
		BT_QUEST = 71,
		BT_INVENTORY = 72,
		BT_SKILL = 73,
		BT_FARM = 74,
		BT_KEYMAP = 75,
		BT_CALLGM = 76,
		BT_CHAT = 77,
		BT_EQUIPS = 78,
		BT_PETEQUIP = 85
	};

	enum textfieldid : short
	{
		TXT_ACC = 0,
		TXT_PASS = 1
	};

	class uielement
	{
	protected:
		map<short, button> buttons;
		map<short, textfield> textfields;
		vector<sprite> sprites;
		vector2d position;
		vector2d dimensions;
		bool active;
		bool visible;
	public:
		uielement() {}
		virtual ~uielement() {}
		virtual void buttonpressed(short) {}
		virtual void draw(ID2D1HwndRenderTarget*);
		virtual void update();
		virtual pair<vector2d, vector2d> bounds();
		void setbutton(short, string);
		void togglehide();
		char sendmouse(vector2d, char);
		bool isactive();
	};
}

