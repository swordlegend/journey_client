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
#include "uielement.h"
#include "Journey.h"

namespace io
{
	void uielement::setbutton(short id, string state)
	{
		buttons[id].setstate(state);
	}

	void uielement::draw(ID2D1HwndRenderTarget* target)
	{
		if (!visible || !active)
			return;

		for (vector<sprite>::iterator itspr = sprites.begin(); itspr != sprites.end(); itspr++)
		{
			itspr->draw(target, position);
		}
		for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
		{
			itbt->second.draw(target, position);
		}
	}

	void uielement::update()
	{
		if (!visible || !active)
			return;

		for (vector<sprite>::iterator itspr = sprites.begin(); itspr != sprites.end(); itspr++)
		{
			itspr->update();
		}
	}

	void uielement::togglehide()
	{
		active = !active;
	}

	pair<vector2d, vector2d> uielement::bounds()
	{
		return pair<vector2d, vector2d>(position, dimensions);
	}

	bool uielement::isactive()
	{
		return active;
	}

	char uielement::sendmouse(vector2d pos, char state)
	{
		if (!active)
			return state;

		char ret = state;
		bool anycoll = false;
		for (map<short, button>::iterator itbt = buttons.begin(); itbt != buttons.end(); itbt++)
		{
			string btst = itbt->second.getstate();
			if (state == -1)
			{
				if (btst == "mouseOver")
					itbt->second.setstate("normal");
				continue;
			}
			if (util::colliding(pos, itbt->second.bounds(), position))
			{
				anycoll = true;
				switch (state)
				{
				case 0:
				case 1:
					if (btst == "normal")
					{
						itbt->second.setstate("mouseOver");
						ret = 1;
					}
					break;
				case 12:
					if (btst == "normal" || btst == "mouseOver")
					{
						itbt->second.setstate("pressed");
						buttonpressed(itbt->first);
					}
				}
			}
			else
			{
				switch (state)
				{
				case 0:
				case 1:
					if (btst == "mouseOver")
					{
						itbt->second.setstate("normal");
					}
					break;
				}
			}
		}
		bool anytext = false;
		for (map<short, textfield>::iterator txtit = textfields.begin(); txtit != textfields.end(); txtit++)
		{
			if (util::colliding(pos, txtit->second.bounds(), position))
			{
				switch (state)
				{
				case 12:
					txtit->second.setstate("active");
					app.getui()->settextfield(&txtit->second);
					anytext = true;
					break;
				}
			}
			else
			{
				switch (state)
				{
				case 12:
					txtit->second.setstate("inactive");
					if (!anytext)
						app.getui()->settextfield(NULL);
					break;
				}
			}
		}
		if (!anycoll && state == 1)
		{
			ret = 0;
		}
		return ret;
	}
}
