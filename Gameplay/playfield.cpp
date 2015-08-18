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
#include "playfield.h"
#include "Journey.h"

namespace gameplay
{
	bool playfield::moveup()
	{
		pair<int, string> warpinfo = m_map.getportal();
		if (warpinfo.first == m_map.getid())
		{
			playerchar.setposition(m_map.getspawnp(warpinfo.second));
			return true;
		}
		else if (warpinfo.first != 0)
		{
			packet_c.changemap(false, warpinfo.first, warpinfo.second, false);
			return false;
		}
	}

	void playfield::draw(ID2D1HwndRenderTarget* target)
	{
		if (step == GST_GAME)
		{
			view.update(vector2d(408, (624 / 1.5)) - playerchar.getposition());
			m_map.draw(target, view.getposition());
			playerchar.draw(target, view.getposition());
		}
	}

	void playfield::update()
	{
		if (step == GST_GAME)
		{
			playerchar.update();
			m_map.update(playerchar.getposition());
		}
	}

	void playfield::changemap(maplemap newmap, char pid)
	{
		step = GST_TRANSITION;

		m_map = newmap;
		playerchar.setmap(&m_map);

		vector2d startpos = m_map.getspawnp(pid);
		playerchar.setposition(startpos);
		view.setposition(vector2d(816 / 2, (624 / 1.5)) - startpos);
		view.setbounds(m_map.getwalls(), m_map.getborders());

		step = GST_GAME;
	}

	void playfield::setplayer(player plchar)
	{
		step = GST_TRANSITION;

		m_account.clear();
		playerchar = plchar;
	}
}
