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
#include "mapbackgrounds.h"

namespace gameplay
{
	mapbackgrounds::mapbackgrounds(nxprovider* provider, nl::node bgnodes, vector2d mapwalls, vector2d mapborders)
	{
		nl::node back = nx::nodes["Back"].resolve("Back/");

		for (nl::node backnode = bgnodes.begin(); backnode != bgnodes.end(); backnode++)
		{
			string bS = backnode.resolve("bS").get_string();
			if (!bS.empty())
			{
				char layer = static_cast<char>(stoi(backnode.name()));
				bgtype type = static_cast<bgtype>(backnode.resolve("type").get_integer());
				bool ani = backnode.resolve("ani").get_bool();
				bool f = backnode.resolve("f").get_bool();

				nl::node spritenode = back.resolve(bS + ".img/" + ((ani) ? "ani/" : "back/") + to_string(backnode.resolve("no").get_integer()));

				vector2d pos = vector2d(backnode.resolve("x").get_integer(), backnode.resolve("y").get_integer());
				vector2d rpos = vector2d(backnode.resolve("rx").get_integer(), backnode.resolve("ry").get_integer());
				vector2d cpos = vector2d(backnode.resolve("cx").get_integer(), backnode.resolve("cy").get_integer());
				byte alpha = static_cast<byte>(backnode.resolve("a").get_integer());

				background bgobj = background(provider->loadanimation(spritenode), type, f, pos, rpos, cpos, mapwalls, mapborders, alpha);

				if (backnode.resolve("front").get_bool())
					foregrounds.push_back(bgobj);
				else
					backgrounds.push_back(bgobj);
			}
		}
	}

	void mapbackgrounds::drawbackgrounds(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		for (vector<background>::iterator back = backgrounds.begin(); back != backgrounds.end(); ++back)
		{
			back->draw(target, viewpos);
		}
	}

	void mapbackgrounds::drawforegrounds(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		for (vector<background>::iterator fore = foregrounds.begin(); fore != foregrounds.end(); ++fore)
		{
			fore->draw(target, viewpos);
		}
	}

	void mapbackgrounds::update()
	{
		for (vector<background>::iterator back = backgrounds.begin(); back != backgrounds.end(); ++back)
		{
			back->update();
		}
		for (vector<background>::iterator fore = foregrounds.begin(); fore != foregrounds.end(); ++fore)
		{
			fore->update();
		}
	}
}