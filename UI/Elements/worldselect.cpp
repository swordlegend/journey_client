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
#include "worldselect.h"
#include "Journey.h"

namespace io
{
	worldselect::worldselect(char num, char* chloads)
	{
		app.getimgcache()->clearcache(ict_login);
		app.getimgcache()->setmode(ict_login);
		nl::nx::view_file("UI");
		nl::nx::view_file("Back");
		node back = nl::nx::nodes["Back"].resolve("Back/login.img/back/");
		node login = nl::nx::nodes["UI"].resolve("Login.img/");
		node worlds = login.resolve("WorldSelect/BtWorld/release/");

		sprites.push_back(sprite(animation(back.resolve("11")), vector2d(370, 300)));
		sprites.push_back(sprite(animation(worlds.resolve("layer:bg")), vector2d(650, 45)));
		sprites.push_back(sprite(animation(login.resolve("Common/frame")), vector2d(400, 290)));

		buttons.insert(make_pair(BT_WORLDSEL0, button(worlds.resolve("button:15"), 650, 20)));

		sprites.push_back(sprite(animation(worlds.resolve("button:16/normal")), vector2d(650, 74)));

		for (char i = 14; i >= 0; i--)
		{
			sprites.push_back(sprite(animation(worlds.resolve("button:" + to_string(i) + "/normal")), vector2d(650, 47)));
		}

		node channels = login.resolve("WorldSelect/BtChannel/");

		sprites.push_back(sprite(animation(channels.resolve("layer:bg")), vector2d(200, 170)));
		sprites.push_back(sprite(animation(channels.resolve("release/layer:15")), vector2d(200, 170)));

		for (char i = 0; i < num; i++)
		{
			button chi = button(
				texture(channels.resolve("button:" + to_string(i) + "/normal/0")),
				texture(channels.resolve("button:" + to_string(i) + "/keyFocused/0")),
				200, 170);
			if (i == 0)
				chi.setstate("pressed");
			buttons.insert(make_pair(BT_CHANNELSEL0 + i, chi));
		}

		buttons.insert(make_pair(BT_GOWORLD, button(channels.resolve("button:GoWorld"), 200, 170)));

		nl::nx::unview_file("Back");
		nl::nx::unview_file("UI");
		app.getimgcache()->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(800, 600);
		active = true;
		visible = true;
		worldid = 0;
		channelid = 0;
	}

	void worldselect::buttonpressed(short id)
	{
		if (id >= BT_WORLDSEL0 && id <= BT_WORLDSEL19)
		{
			worldid = id - BT_WORLDSEL0;
			for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
			{
				if (itbt->first != id && itbt->first >= BT_WORLDSEL0 && itbt->first <= BT_WORLDSEL19 && itbt->second.getstate() == "pressed")
					itbt->second.setstate("normal");
			}
			return;
		}
		else if (id >= BT_CHANNELSEL0 && id <= BT_CHANNELSEL16)
		{
			channelid = id - BT_CHANNELSEL0;
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
			app.getui()->getfield()->setworldchannel(worldid, channelid);
			packet_c.charlrequest(worldid, channelid);
		}
	}
}
