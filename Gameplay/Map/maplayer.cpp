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
#include "maplayer.h"

namespace gameplay
{
	maplayer::maplayer(node source)
	{
		string tS = source["info"]["tS"].get_string();
		for (node layernode = source.begin(); layernode != source.end(); layernode++)
		{
			if (layernode.name() == "obj")
			{
				for (node objnode = layernode.begin(); objnode != layernode.end(); objnode++)
				{
					vector2d position = vector2d(
						static_cast<int>(objnode.resolve("x").get_integer()),
						static_cast<int>(objnode.resolve("y").get_integer()));
					int z = static_cast<int>(objnode.resolve("z").get_integer());
					node bmpnode = nx::nodes["Obj"].resolve("Obj/" + objnode.resolve("oS").get_string() + ".img/" + objnode.resolve("l0").get_string() + "/" + objnode.resolve("l1").get_string() + "/" + objnode.resolve("l2").get_string());
					objs[z].push_back(sprite(animation(bmpnode), position, true, objnode.resolve("f").get_bool()));
				}
			}
			else if (layernode.name() == "tile")
			{
				for (node tilenode = layernode.begin(); tilenode != layernode.end(); tilenode++)
				{
					vector2d position = vector2d(
						static_cast<int>(tilenode.resolve("x").get_integer()),
						static_cast<int>(tilenode.resolve("y").get_integer()));
					int zM = static_cast<int>(tilenode.resolve("zM").get_integer());
					node bmpnode = nx::nodes["Tile"].resolve("Tile/" + tS + ".img/" + tilenode.resolve("u").get_string() + "/" + to_string(tilenode.resolve("no").get_integer()));
					tiles[zM].push_back(sprite(animation(bmpnode), position));
				}
			}
		}

	}

	void maplayer::draw(ID2D1HwndRenderTarget* target, vector2d viewpos)
	{
		for (map<char, vector<sprite>>::iterator obj = objs.begin(); obj != objs.end(); ++obj)
		{
			for (vector<sprite>::iterator objit = obj->second.begin(); objit != obj->second.end(); ++objit)
			{
				objit->draw(target, viewpos);
			}
		}

		for (map<char, vector<sprite>>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
		{
			for (vector<sprite>::iterator tlit = tile->second.begin(); tlit != tile->second.end(); ++tlit)
			{
				tlit->draw(target, viewpos);
			}
		}
	}

	void maplayer::update()
	{
		for (map<char, vector<sprite>>::iterator obj = objs.begin(); obj != objs.end(); ++obj)
		{
			for (vector<sprite>::iterator objit = obj->second.begin(); objit != obj->second.end(); ++objit)
			{
				objit->update();
			}
		}

		for (map<char, vector<sprite>>::iterator tile = tiles.begin(); tile != tiles.end(); ++tile)
		{
			for (vector<sprite>::iterator tlit = tile->second.begin(); tlit != tile->second.end(); ++tlit)
			{
				tlit->update();
			}
		}
	}
}
