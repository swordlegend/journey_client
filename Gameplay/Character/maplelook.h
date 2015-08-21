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
#include "stdfax.h"
#include "texture.h"

using namespace graphics;
using namespace std;
using namespace util;

namespace gameplay
{
	enum charlayers : char
	{
		CL_BASE,
		CL_HAIRBBODY,
		CL_CAPE,
		CL_BODY,
		CL_SHOES,
		CL_PANTS,
		CL_TOP,
		CL_MAIL,
		CL_ARM,
		CL_MAILARM,
		CL_LHAND,
		CL_GLOVE,
		CL_HAIR,
		CL_EARRINGS,
		CL_ARMOHAIR,
		CL_HEAD,
		CL_FACE,
		CL_HAIRSHADE,
		CL_FACEACC,
		CL_EYEACC,
		CL_HAIROHEAD,
		CL_HAT,
		CL_SHIELD,
		CL_WEAPON,
		CL_RHAND,
		CL_WEAPONOHAND,
		CL_RGLOVE,
		CL_WEAPONOGLOVE,
		CL_HANDOWEP
	};

	struct charsprites
	{
		map<string, map<char, map<charlayers, texture>>> textures;

		void insert (charsprites& cs)
		{
			for (map<string, map<char, map<charlayers, texture>>>::iterator insit = textures.begin(); insit != textures.end(); insit++)
			{
				string state = insit->first;
				for (map<char, map<charlayers, texture>>::iterator insit_b = insit->second.begin(); insit_b != insit->second.end(); insit_b++)
				{
					char frame = insit_b->first;
					textures[state][frame].insert(cs.textures[state][frame].begin(), cs.textures[state][frame].end());
				}
			}
		}
	};

	typedef struct charsprites charsprites;

	class maplelook
	{
	public:
		bool twohweapon;
		bool female;
		char skin;
		int faceid;
		int hairid;
		map<char, int> equips;
		map<char, int> maskedequips;
		vector<int> petids;
		maplelook() {}
		~maplelook() {}
		maplelook(bool, char, int, int, map<char, int>, map<char, int>, vector<int>);
		string getstate();
		void addsprites(charsprites, map<string, map<char, short>>, map<string, map<char, short>>, map<string, map<char, pair<string, char>>>, map<string, map<char, map<charlayers, map<string, vector2d>>>>);
		void draw(ID2D1HwndRenderTarget*, vector2d);
		bool update();
		void setposition(vector2d);
		void setfleft(bool);
		void setstate(string);
		void setexpression(char);
	private:
		map<string, map<char, short>> bodydelays;
		map<string, map<char, short>> facedelays;
		map<string, map<char, pair<string, char>>> bodyactions;
		map<string, map<char, map<charlayers, map<string, vector2d>>>> bodyheadmap;
		charsprites sprites;
		vector2d position;
		string expression;
		string state;
		char frame;
		char faceframe;
		short elapsed;
		short elapsedf;
		bool faceleft;
		float scalex;
	};
}

