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
#include "mapbackgrounds.h"
#include "nxfile.h"

namespace gameplay
{
	mapbackgrounds::mapbackgrounds(node bgnodes, vector2d mapwalls, vector2d mapborders)
	{
		node back = nx::nodes["Back"]["Back"];

		for (node backnode = bgnodes.begin(); backnode != bgnodes.end(); ++backnode)
		{
			if (!backnode["bS"].get_string().empty())
			{
				if (backnode["front"].get_bool())
				{
					foregrounds.push_back(background(backnode, back, mapwalls, mapborders));
				}
				else
				{
					backgrounds.push_back(background(backnode, back, mapwalls, mapborders));
				}
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