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
#include "uibar.h"

namespace io
{
	uibar::uibar(texture f, texture m, texture e, int max, vector2d pos)
	{
		front = f;
		barmid = m;
		barend = e;
		barmax = max;
		position = pos;
	}

	void uibar::draw(ID2D1HwndRenderTarget* target, vector2d pos, int value, int max)
	{
		vector2d absp = pos + position;
		int percent = ((value / 100) * barmax) / (max / 100);
		front.draw(absp);
		barmid.draw(absp + vector2d(1, 0), vector2d(percent, 0));
		barend.draw(absp + vector2d(percent + 1, 0));
	}
}
