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
#include "login.h"
#include "Journey.h"

namespace io
{
	login::login()
	{
		app.getimgcache()->setmode(ict_login);
		nl::nx::view_file("UI");

		node title = nl::nx::nodes["UI"].resolve("Login.img/Title/");
		node common = nl::nx::nodes["UI"].resolve("Login.img/Common/");

		sprites.push_back(sprite(animation(title.resolve("11")), vector2d(410, 300)));
		sprites.push_back(sprite(animation(title.resolve("35")), vector2d(410, 260)));
		sprites.push_back(sprite(animation(title.resolve("Logo")), vector2d(410, 130)));
		sprites.push_back(sprite(animation(title.resolve("signboard")), vector2d(410, 300)));
		sprites.push_back(sprite(animation(common.resolve("frame")), vector2d(400, 290)));

		buttons[BT_LOGIN] = button(title.resolve("BtLogin"), 475, 248);
		buttons[BT_NEW] = button(title.resolve("BtNew"), 309, 320);
		buttons[BT_HOMEPAGE] = button(title.resolve("BtHomePage"), 382, 320);
		buttons[BT_PWDLOST] = button(title.resolve("BtPasswdLost"), 470, 300);
		buttons[BT_QUIT] = button(title.resolve("BtQuit"), 455, 320);
		buttons[BT_LOGINLOST] = button(title.resolve("BtLoginIDLost"), 395, 300);
		buttons[BT_SAVEID] = button(title.resolve("BtLoginIDSave"), 325, 300);

		bool saveid = config.accsaved();
		saveidcheck = sprite(animation(title.resolve("check")), vector2d(313, 304));

		string defaultacc;
		if (saveid)
			defaultacc = config.getdefaultacc();
		else
			defaultacc = "";

		textfields[TXT_ACC] = textfield(app.getfonts()->getfont(DWF_LEFT), TXC_WHITE, defaultacc, vector2d(315, 249), 12);
		textfields[TXT_ACC].setbg(texture(title.resolve("ID")), -5, 0);

		textfields[TXT_PASS] = textfield(app.getfonts()->getfont(DWF_LEFT), TXC_WHITE, "", vector2d(315, 275), 12);
		textfields[TXT_PASS].setbg(texture(title.resolve("PW")), -5, 0);

		if (!saveid)
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
		app.getimgcache()->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(800, 600);
		active = true;
		visible = true;
	}

	void login::draw(ID2D1HwndRenderTarget* target)
	{
		uielement::draw(target);

		saveidcheck.setframe((saveid) ? 0 : 1);
		saveidcheck.draw(target, position);

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
			if (saveid)
				config.save(3, textfields[TXT_ACC].text());
			app.getui()->add(UI_LOGINWAIT);
			packet_c.c_login(textfields[TXT_ACC].text(), textfields[TXT_PASS].text());
			return;
		case BT_QUIT:
			quit();
			return;
		case BT_SAVEID:
			saveid = !saveid;
			buttons[BT_SAVEID].setstate("mouseOver");
			return;
		}
	}
}
