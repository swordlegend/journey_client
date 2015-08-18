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
#include "equipinventory.h"

namespace io
{
	equipinventory::equipinventory(nxprovider* provider, inventory* inv)
	{
		equipped = inv->getequipped();
		equippedcash = inv->getequippedcash();

		provider->setcmode(CM_SYS);
		nl::nx::view_file("UI");

		/*node source = nl::nx::nodes["UI"].resolve("UIWindow2.img/Equip/character");
		node petsource = nl::nx::nodes["UI"].resolve("UIWindow2.img/Equip/pet");

		sprite back = provider->loadsprite(source.resolve("backgrnd"));
		back.setposition(vector2d(0, 0));
		sprites.push_back(back);
		back = provider->loadsprite(source.resolve("backgrnd2"));
		back.setposition(vector2d(0, 0));
		sprites.push_back(back);
		back = provider->loadsprite(source.resolve("backgrnd3_Kanna"));
		back.setposition(vector2d(0, 0));
		sprites.push_back(back);

		sprite newslot = provider->loadsprite(source.resolve("cashPendant"));
		newslot.setposition(vector2d(0, 0));
		sprites.push_back(newslot); 
		newslot = provider->loadsprite(source.resolve("charmPocket"));
		newslot.setposition(vector2d(0, 0));
		sprites.push_back(newslot);
		newslot = provider->loadsprite(source.resolve("emblem"));
		newslot.setposition(vector2d(0, 0));
		sprites.push_back(newslot);

		buttons.insert(make_pair(BT_PETEQUIP, button(provider->loadbutton(source.resolve("BtPet")), 0, 0)));

		sprite petbg = provider->loadsprite(petsource.resolve("backgrnd"));
		petbg.setposition(vector2d(back.getdimension().x(), 0));
		petsprites.push_back(petbg);
		petbg = provider->loadsprite(petsource.resolve("backgrnd2"));
		petbg.setposition(vector2d(back.getdimension().x(), 0));
		petsprites.push_back(petbg);
		petbg = provider->loadsprite(petsource.resolve("backgrnd3"));
		petbg.setposition(vector2d(back.getdimension().x(), 0));
		petsprites.push_back(petbg);*/

		nl::nx::unview_file("UI");
		provider->unlock();
		position = vector2d(0, 0);
		//dimensions = back.getdimension();
		active = true;
		visible = true;
		showpet = false;
	}

	equipinventory::~equipinventory()
	{
		delete equipped;
		delete equippedcash;
	}

	void equipinventory::draw(ID2D1HwndRenderTarget* target)
	{
		if (active)
		{
			uielement::draw(target);

			if (showpet)
			{
				for (vector<sprite>::iterator petit = petsprites.begin(); petit != petsprites.end(); petit++)
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
			break;
		}
	}
}
