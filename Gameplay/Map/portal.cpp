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
#include "portal.h"

namespace gameplay
{
	portal::portal(){}
	portal::~portal(){}

	portal::portal(portaltype tp, string name, int tid, bool in, string tpn, animation ani, vector2d pos)
	{
		type = tp;
		pname = name;
		targetid = tid;
		intermap = in;
		targetpname = tpn;
		anim = ani;
		position = pos;
		touched = false;
	}

	void portal::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		if (type == PT_REGULAR)
		{
			anim.draw(target, position + parentpos);
		}
		else if (type == PT_HIDDEN || type == PT_SCRIPTED_HIDDEN)
		{
			if (touched)
			{
				anim.draw(target, position + parentpos);
			}
		}
	}

	void portal::update()
	{
		anim.update(8);
	}

	void portal::settouch(bool t)
	{
		touched = t;
	}

	vector2d portal::getposition()
	{
		return position;
	}

	vector2d portal::getdimension()
	{
		return vector2d(30, 50);
	}

	portaltype portal::gettype()
	{
		return type;
	}

	pair<int, string> portal::getwarpinfo()
	{
		if (intermap)
			return make_pair(targetid, targetpname);
		else
			return make_pair(targetid, pname);
	}

	string portal::getname()
	{
		return pname;
	}
}
