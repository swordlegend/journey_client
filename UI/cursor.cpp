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
#include "cursor.h"
#include "Journey.h"

namespace io
{
	cursor::cursor()
	{
		position = vector2d();
		state = MST_IDLE;
	}

	cursor::~cursor(){}

	void cursor::init(nxprovider* provider)
	{
		provider->setcmode(data::CM_SYS);
		nl::nx::view_file("UI");
		node cursornode = nl::nx::nodes["UI"].resolve("Basic.img/Cursor/");
		for (mousestate i = MST_IDLE; i <= MST_RCLICK; i = (mousestate) (i + 1))
		{
			sprites[i] = provider->loadanimation(cursornode.resolve(to_string(i)));
		}
		nl::nx::unview_file("UI");
		provider->unlock();
	}

	void cursor::update(vector2d pos)
	{
		position = pos;
	}

	void cursor::draw(ID2D1HwndRenderTarget* target)
	{
		sprites[state].draw(target, position);
	}

	void cursor::update()
	{
		sprites[state].update();
	}

	char cursor::getstate()
	{
		return state;
	}

	vector2d cursor::getposition()
	{
		return position;
	}

	void cursor::setstate(char st)
	{
		state = (mousestate)st;
	}
}