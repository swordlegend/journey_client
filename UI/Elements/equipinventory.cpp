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
#include "equipinventory.h"
#include "Journey.h"

namespace io
{
	equipinventory::equipinventory(inventory* inv)
	{
		iconpositions = new vector2d[13];
		iconpositions[0] = vector2d(24, 24);
		iconpositions[1] = vector2d(43, 58);
		iconpositions[2] = vector2d(76, 24);
		iconpositions[3] = vector2d(109, 157);
		iconpositions[4] = vector2d(109, 122);
		iconpositions[5] = vector2d(43, 157);
		iconpositions[6] = vector2d(43, 190);
		iconpositions[7] = vector2d(75, 223);
		iconpositions[8] = vector2d(10, 190);
		iconpositions[9] = vector2d(24, 96);
		iconpositions[10] = vector2d(48, 96);
		iconpositions[11] = vector2d(109, 157);
		iconpositions[12] = vector2d();
		iconpositions[13] = vector2d();

		invent = inv;

		for (short i = 0; i < 14; i++)
		{
			mapleequip* todraw = invent->getequip(i);
			if (todraw)
			{
				int id = todraw->getid();
				equips[i] = icon(id, false);
			}
		}

		app.getimgcache()->setmode(ict_sys);
		nl::nx::view_file("UI");

		node source = nl::nx::nodes["UI"].resolve("UIWindow2.img/Equip/character");
		node petsource = nl::nx::nodes["UI"].resolve("UIWindow2.img/Equip/pet");

		sprites.push_back(sprite(animation(source.resolve("backgrnd")), vector2d()));
		sprites.push_back(sprite(animation(source.resolve("backgrnd2")), vector2d()));
		sprites.push_back(sprite(animation(source.resolve("backgrnd3_Kanna")), vector2d()));
		sprites.push_back(sprite(animation(source.resolve("cashPendant")), vector2d()));
		sprites.push_back(sprite(animation(source.resolve("charmPocket")), vector2d()));
		sprites.push_back(sprite(animation(source.resolve("emblem")), vector2d()));

		buttons.insert(make_pair(BT_PETEQUIP, button(source.resolve("BtPet"), 0, 0)));

		petsprites.push_back(sprite(animation(petsource.resolve("backgrnd")), vector2d(184, 0)));
		petsprites.push_back(sprite(animation(petsource.resolve("backgrnd2")), vector2d(184, 0)));
		petsprites.push_back(sprite(animation(petsource.resolve("backgrnd3")), vector2d(184, 0)));

		nl::nx::unview_file("UI");
		app.getimgcache()->unlock();
		position = vector2d(0, 0);
		dimensions = vector2d(184, 290);
		active = true;
		visible = true;
		showpet = false;
	}

	equipinventory::~equipinventory()
	{
		delete invent;
		delete iconpositions;
	}

	void equipinventory::draw(ID2D1HwndRenderTarget* target)
	{
		if (active)
		{
			uielement::draw(target);

			for (map<short, icon>::iterator eqit = equips.begin(); eqit != equips.end(); ++eqit)
			{
				eqit->second.draw(position + iconpositions[eqit->first], 1.0f);
			}

			if (showpet)
			{
				for (vector<sprite>::iterator petit = petsprites.begin(); petit != petsprites.end(); ++petit)
				{
					petit->draw(target, position);
				}
			}
		}
	}

	void equipinventory::update()
	{

	}

	void equipinventory::buttonpressed(short id)
	{
		switch (id)
		{
		case BT_PETEQUIP:
			showpet = !showpet;
			buttons[BT_PETEQUIP].setstate("mouseOver");
			break;
		}
	}
}
