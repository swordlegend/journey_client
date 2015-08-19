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
#include "mob.h"

namespace gameplay
{
	mob::mob(map<string, animation> t, string n, short l, short s)
	{
		textures = t;
		name = n;
		level = l;
		speed = s;
		state = "stand";
		moved = 0;
		pos = vector2d();
		walls = vector2d();
	}

	void mob::setinfo(int id, vector2d p, vector2d w, char s, short f, char e, bool fd, char t)
	{
		oid = id;
		pos = p;
		walls = w;
		stance = s;
		fh = f;
		effect = e;
		fadein = fd;
		team = t;
	}

	void mob::update()
	{
		textures[state].update();

		if (speed != 0)
		{

		}
	}

	void mob::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		textures[state].draw(target, parentpos + pos);
	}
}
