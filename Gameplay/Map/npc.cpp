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
#include "npc.h"


namespace gameplay
{
	npc::npc() {}

	npc::~npc() {}

	npc::npc(map<string, animation> t, map<string, vector<string>> l, string n, string f)
	{
		textures = t;
		lines = l;
		name = n;
		func = f;
		state = "stand";
	}

	void npc::setinfo(int o, bool fr, short f, vector2d pos)
	{
		oid = o;
		front = fr;
		fh = f;
		position = pos;
	}

	void npc::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		textures[state].draw(target, parentpos + position);
	}

	void npc::update()
	{
		textures[state].update();
	}
}
