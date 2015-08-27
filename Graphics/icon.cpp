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
#include "icon.h"
#include "Journey.h"

namespace graphics
{
	icon::icon(int itemid, bool r)
	{
		app.getimgcache()->setmode(ict_sys);

		bool equip = itemid < 2000000;
		string category;

		if (equip)
		{
			nx::view_file("Character");

			int prefix = itemid / 10000;

			switch (prefix)
			{
			case 100:
				category = "Cap";
				break;
			case 101:
			case 102:
			case 103:
				category = "Accessory";
				break;
			case 104:
				category = "Coat";
				break;
			case 105:
				category = "Longcoat";
				break;
			case 106:
				category = "Pants";
				break;
			case 107:
				category = "Shoes";
				break;
			case 108:
				category = "Glove";
				break;
			case 109:
				category = "Shield";
				break;
			case 110:
				category = "Cape";
				break;
			default:
				category = "Weapon";
			}

			node src = nx::nodes["Character"][category]["0" + to_string(itemid) + ".img"]["info"];
			textures[false] = texture(src["icon"]);
			textures[true] = texture(src["iconRaw"]);

			nx::unview_file("Character");
		}
		else
		{
			nx::view_file("Item");

			switch (itemid / 1000000)
			{
			case 2:
				category = "Consume";
				break;
			case 3:
				category = "Install";
				break;
			case 4:
				category = "Etc";
				break;
			case 5:
				category = "Cash";
				break;
			}

			node src = nx::nodes["Item"][category]["0" + to_string(itemid / 10000) + ".img"]["0" + to_string(itemid)]["info"];
			textures[false] = texture(src["icon"]);
			textures[true] = texture(src["iconRaw"]);

			nx::unview_file("Item");
		}
		app.getimgcache()->unlock();

		pos = vector2d();
		raw = r;
	}

	icon::icon(node src, bool r)
	{
		textures[false] = texture(src["icon"]);
		textures[true] = texture(src["iconRaw"]);
		pos = vector2d();
		raw = r;
	}

	void icon::draw(vector2d parentpos, float alpha)
	{
		textures[raw].draw(pos + parentpos, alpha);
	}
}
