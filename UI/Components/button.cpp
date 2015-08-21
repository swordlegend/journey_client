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
#include "button.h"

namespace io
{
	button::button(node src, int x, int y)
	{
		sprites["pressed"] = texture(src.resolve("pressed/0"));
		sprites["mouseOver"] = texture(src.resolve("mouseOver/0"));
		sprites["normal"] = texture(src.resolve("normal/0"));
		sprites["disabled"] = texture(src.resolve("disabled/0"));
		position = vector2d(x, y);
		state = "normal";
		bttype = BTT_REGULAR;
	}
	
	button::button(texture s1, texture s2, int x, int y)
	{
		sprites["normal"] = s1;
		sprites["select"] = s2;
		position = vector2d(x, y);
		state = "normal";
		bttype = BTT_ONESPRITE;
	}

	button::button(int x, int y, int w, int h)
	{
		position = vector2d(x, y);
		dimension = vector2d(w, h);
		state = "normal";
		bttype = BTT_AREA;
	}

	std::pair<vector2d, vector2d> button::bounds()
	{
		if (bttype == BTT_AREA)
			return std::pair<vector2d, vector2d>(position, dimension);
		else
			return std::pair<vector2d, vector2d>(position - sprites["normal"].getorigin(), (sprites["normal"].getdimension()));
	}

	void button::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		vector2d absp = position + parentpos;

		if (bttype == BTT_ONESPRITE)
		{
			if (state == "normal")
			{
				sprites["normal"].draw(absp);
			}
			else if (state == "pressed" || state == "mouseOver")
			{
				sprites["normal"].draw(absp);
				sprites["select"].draw(absp);
			}
		}
		else if (bttype == BTT_REGULAR)
		{
			sprites[state].draw(absp);
		}
	}

	std::string button::getstate()
	{
		return state;
	}

	void button::setstate(std::string st)
	{
		state = st;
	}
}
