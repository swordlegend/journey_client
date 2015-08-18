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
#include "texture.h"

using namespace std;

namespace graphics
{
	const short DPF = 16;

	class animation
	{
	public:
		animation() {}
		animation(map<byte, texture>, map<byte, short>, map<byte, pair<byte, byte>>);
		~animation() {}
		void draw(ID2D1HwndRenderTarget*, vector2d);
		void draw(ID2D1HwndRenderTarget*, vector2d, float);
		bool update() { return update(DPF); }
		bool update(short);
		void setframe(byte);
		vector2d getdimension(byte);
	protected:
		map<byte, texture> textures;
		map<byte, short> delays;
		map<byte, pair<byte, byte>> alphablends;
		byte frame;
		byte last_f;
		short elapsed;
		float alphastep;
		float alpha;
	};
}

