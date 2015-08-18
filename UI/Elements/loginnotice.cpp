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
#include "loginnotice.h"
#include "Journey.h"

namespace io
{
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
}
