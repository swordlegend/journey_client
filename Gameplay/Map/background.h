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
#include "animation.h"

using namespace graphics;

namespace gameplay
{
	enum bgtype : char
	{
		BGT_NORMAL,
		BGT_HTILED,
		BGT_VTILED,
		BGT_TILED,
		BGT_HMOVEA,
		BGT_VMOVEA,
		BGT_HMOVEB,
		BGT_VMOVEB
	};

	class background
	{
	private:
		animation sprite;
		bgtype type;
		vector2d pos;
		vector2d rpos;
		vector2d cpos;
		vector2d mapwalls;
		vector2d mapborders;
		vector2d movement;
		float alpha;
		bool flipped;
	public:
		background(animation, bgtype, bool, vector2d, vector2d, vector2d, vector2d, vector2d, byte);
		~background() {}
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void update();
	};
}

