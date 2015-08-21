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
#include "ui.h"
#include "login.h"
#include "loginwait.h"
#include "loginnotice.h"
#include "worldselect.h"
#include "charselect.h"
#include "statusbar.h"
#include "equipinventory.h"

namespace io
{
	void ui::init()
	{
		uilock = SRWLOCK_INIT;

		mouse.init();

		shift = false;
		actionsenabled = true;
	}

	void ui::add(char type)
	{
		add(type, 0);
	}

	void ui::add(char type, char param)
	{
		if (elements[type])
		{
			remove(type);
		}

		AcquireSRWLockExclusive(&uilock);

		uielement* toadd = 0;
		switch (type)
		{
		case UI_LOGIN:
			toadd = new login();
			break;
		case UI_LOGINNOTICE:
			toadd = new loginnotice(param);
			actionsenabled = true;
			break;
		case UI_LOGINWAIT:
			toadd = new loginwait();
			break;
		case UI_WORLDSELECT:
			toadd = new worldselect(field.getworlds()->at(0).getchannels(), field.getworlds()->at(0).getchloads());
			actionsenabled = true;
			break;
		case UI_CHARSEL:
			toadd = new charselect(field.getaccount()->getslots(), field.getaccount()->getchars());
			actionsenabled = true;
			break;
		case UI_STATUSBAR:
			toadd = new statusbar(field.getplayer()->getstats());
			break;
		case UI_EQUIPS:
			toadd = new equipinventory(field.getplayer()->getinventory());
			break;
		}
		elements[type] = toadd;

		ReleaseSRWLockExclusive(&uilock);
	}

	void ui::remove(char type)
	{
		AcquireSRWLockExclusive(&uilock);

		delete elements[type];
		elements.erase(type);

		ReleaseSRWLockExclusive(&uilock);
	}

	void ui::draw(ID2D1HwndRenderTarget* target)
	{
		field.draw(target);
		if (TryAcquireSRWLockShared(&uilock))
		{
			for (map<char, uielement*>::iterator elit = elements.begin(); elit != elements.end(); elit++)
			{
				elit->second->draw(target);
			}
			ReleaseSRWLockShared(&uilock);
		}
		mouse.draw(target);
	}

	void ui::update()
	{
		field.update();
		if (TryAcquireSRWLockShared(&uilock))
		{
			for (map<char, uielement*>::iterator elit = elements.begin(); elit != elements.end(); elit++)
			{
				elit->second->update();
			}
			ReleaseSRWLockShared(&uilock);
		}
		mouse.update();
	}

	void ui::sendmouse(vector2d pos)
	{
		sendmouse(mouse.getstate(), pos);
	}

	void ui::sendkey(WPARAM keycode, bool down)
	{
		if (!actionsenabled)
			return;

		if (activetext != 0)
		{
			if (down)
			{
				if (keycode == VK_SHIFT)
					shift = true;
				else if (keycode == VK_BACK)
					activetext->sendchar(0);
			}
			else
			{
				if ((keycode > 47 && keycode < 64) || (keycode > 64 && keycode < 91))
				{
					char letter = (char)keycode;
					if (!shift)
						letter += 32;
					activetext->sendchar(letter);
				}
				else if (keycode == VK_SHIFT)
					shift = false;
				else if (keycode == VK_LEFT)
					activetext->sendchar(1);
				else if (keycode == VK_RIGHT)
					activetext->sendchar(2);
			}
			return;
		}
		switch (keycode)
		{
		case VK_LEFT:
			field.getplayer()->move(MC_LEFT, down);
			break;
		case VK_RIGHT:
			field.getplayer()->move(MC_RIGHT, down);
			break;
		case VK_UP:
			if (down)
			{
				actionsenabled = field.moveup();
			}
			break;
		case VK_DOWN:
			field.getplayer()->crouch(down);
			break;
		default:
			pair<keytype, int> mapping = keys.getaction(keycode);
			keytype type = mapping.first;
			int action = mapping.second;
			switch (type)
			{
			case KT_SKILL:
				break;
			case KT_ITEM:
				break;
			case KT_CASH:
				break;
			case KT_MENU:
				if (down)
				{
					char elemtype = -1;
					switch (action)
					{
					case KA_INVENTORY:
						elemtype = UI_INVENTORY;
						break;
					case KA_EQUIPS:
						elemtype = UI_EQUIPS;
						break;
					}

					if (elemtype != -1)
					{
						if (elements[elemtype])
							elements[elemtype]->togglehide();
						else
							add(elemtype);
					}
				}
				break;
			case KT_ACTION:
				switch (action)
				{
				case KA_JUMP:
					field.getplayer()->move(MC_JUMP, down);
					break;
				case KA_SIT:
					break;
				case KA_ATTACK:
					field.useattack(-1);
					break;
				}
				break;
			case KT_FACE:
				field.getplayer()->setexpression(action - 100);
				break;
			case KT_MACRO:
				break;
			}
		}
	}

	void ui::sendmouse(char param, vector2d pos)
	{
		mouse.update(pos);

		uielement* front = 0;
		if (actionsenabled)
		{
			if (TryAcquireSRWLockShared(&uilock))
			{
				for (map<char, uielement*>::iterator elit = elements.begin(); elit != elements.end(); elit++)
				{
					if (util::colliding(pos, elit->second->bounds()) && elit->second->isactive())
					{
						if (front != 0)
							front->sendmouse(pos, -1);
						front = elit->second;
					}
				}
				ReleaseSRWLockShared(&uilock);
			}
		}

		if (front != 0)
		{
			mouse.setstate(front->sendmouse(pos, param));
		}
		else
		{
			mouse.setstate(param);

			if (mouse.getstate() == 1)
				mouse.setstate(0);
		}
	}

	uielement* ui::getelement(char type)
	{
		return elements[type];
	}

	keyboard* ui::getkeyboard()
	{
		return &keys;
	}

	void ui::settextfield(textfield* txt)
	{
		activetext = txt;
	}

	void ui::enableactions()
	{
		actionsenabled = true;
	}

	void ui::disableactions()
	{
		actionsenabled = false;
	}
}
