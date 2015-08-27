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
#include "maplelook.h"

namespace gameplay
{
	maplelook::maplelook(bool fm, char sk, int fc, int hr, map<char, int> eqs, map<char, int> mskeqs, vector<int> pets)
	{
		female = fm;
		skin = sk;
		faceid = fc;
		hairid = hr;
		equips = eqs;
		maskedequips = mskeqs;
		petids = pets;
		twohweapon = false;
		if (twohweapon)
			state = "stand2";
		else
			state = "stand1";
		expression = "default";
		faceframe = 0;
		frame = 0;
		elapsed = 0;
		elapsedf = 0;
		position = vector2d(0, 0);
		faceleft = true;
	}

	string maplelook::getstate()
	{
		return state;
	}

	void maplelook::addsprites(charsprites t, map<string, map<char, short>> bd, map<string, map<char, short>> fd, map<string, map<char, pair<string, char>>> ba, map<string, map<char, map<charlayers, map<string, vector2d>>>> bhm)
	{
		sprites = t;
		bodydelays = bd;
		facedelays = fd;
		bodyactions = ba;
		bodyheadmap = bhm;
	}

	void maplelook::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		map<charlayers, texture> textures = sprites.textures[state][frame];
		textures.insert(make_pair(CL_FACE, sprites.textures[expression][faceframe][CL_FACE]));

		for (map<charlayers, texture>::iterator txtit = textures.begin(); txtit != textures.end(); ++txtit)
		{
			charlayers chl = txtit->first;
			vector2d absp = parentpos + position;

			if (!faceleft)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(-1.0f, 1.0f),
					D2D1::Point2F(
					(float)absp.x(),
					(float)absp.y())));
			}

			char fr = frame;
			if (chl == CL_FACE)
			{
				vector2d shf = bodyheadmap[state][frame][CL_HEAD]["neck"] - bodyheadmap[state][frame][CL_BODY]["neck"] - bodyheadmap[state][frame][CL_HEAD]["brow"];
				txtit->second.shift(vector2d() - shf);
				txtit->second.draw(absp);
				txtit->second.shift(shf);
			}
			else
			{
				txtit->second.draw(absp);
			}

			if (!faceleft)
			{
				target->SetTransform(
					D2D1::Matrix3x2F::Scale(
					D2D1::Size(1.0f, 1.0f),
					D2D1::Point2F(
					(float)absp.x(),
					(float)absp.y())));
			}
		}
	}

	bool maplelook::update()
	{
		bool aniend = false;

		elapsed += 16;

		short delay = bodydelays[state][frame];
		if (elapsed > delay)
		{
			elapsed -= delay;
			frame = (bodydelays[state].count(frame + 1))? frame + 1 : 0;

			if (frame == 0)
			{
				aniend = true;

				if (state == "stabO1")
				{
					state = "stand1";
				}
			}
		}

		elapsedf += 16;

		delay = facedelays[expression][faceframe];
		if (elapsedf > delay)
		{
			elapsedf -= delay;
			faceframe = (facedelays[expression].count(faceframe + 1)) ? faceframe + 1 : 0;
			if (faceframe == 0)
			{
				if (expression == "default")
					expression = "blink";
				else
					expression = "default";
			}
		}

		return aniend;
	}

	void maplelook::setposition(vector2d pos)
	{
		position = pos;
	}

	void maplelook::setfleft(bool fleft)
	{
		faceleft = fleft;
		scalex = (fleft) ? 1.0f : -1.0f;
	}

	void maplelook::setstate(string st)
	{
		if (st == "stand1" && twohweapon)
			st = "stand2";

		if (state != st)
		{
			frame = 0;
			elapsed = 0;
			state = st;
		}
	}

	void maplelook::setexpression(char exp)
	{
		elapsedf = 0;
		faceframe = 0;
		switch (exp)
		{
		case 0:
			expression = "hit";
			break;
		case 1:
			expression = "smile";
			break;
		case 2:
			expression = "troubled";
			break;
		case 3:
			expression = "cry";
			break;
		case 4:
			expression = "angry";
			break;
		case 5:
			expression = "bewildered";
			break;
		case 6:
			expression = "stunned";
			break;
		}
	}
}
