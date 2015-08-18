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
#include "animation.h"

namespace graphics
{
	animation::animation(map<byte, texture> txt, map<byte, short> del, map<byte, pair<byte, byte>> alp)
	{
		textures = txt;
		delays = del;
		alphablends = alp;
		frame = 0;
		last_f = textures.size() - 1;
		elapsed = 0;
		alpha = static_cast<float>(alphablends[0].first);
		alphastep = (alphablends[0].second - alpha) / (delays[0] / 16);
	}

	void animation::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		textures[frame].draw(target, parentpos, alpha / 255);
	}

	void animation::draw(ID2D1HwndRenderTarget* target, vector2d parentpos, float alpha)
	{
		textures[frame].draw(target, parentpos, alpha);
	}

	bool animation::update(short frames)
	{
		if (last_f > 0)
		{
			elapsed += frames;

			if (alphastep != 0.f)
			{
				alpha += alphastep;
				if (alpha < 0.f)
					alpha = 0.f;
				else if (alpha > 255.f)
					alpha = 255.f;
			}

			short delay = delays[frame];
			if (elapsed > delay)
			{
				elapsed -= delay;

				frame = (frame == last_f) ? 0 : frame + 1;

				float nexta = static_cast<float>(alphablends[frame].second - alpha);
				alphastep = (nexta * DPF) / delay;

				return frame == 0;
			}
		}
		return false;
	}

	void animation::setframe(byte f)
	{
		frame = f;
		elapsed = 0;
	}

	vector2d animation::getdimension(byte f)
	{
		return textures[f].getdimension();
	}
}
