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
	animation::animation(node src)
	{
		if (src.istype(bitmapnode))
		{
			textures[0] = texture(src);
			alphablends[0] = pair<byte, byte>(255, 255);
		}
		else
		{
			for (node nodeit = src.begin(); nodeit != src.end(); nodeit++)
			{
				if (nodeit.istype(bitmapnode))
				{
					byte frame = static_cast<byte>(stoi(nodeit.name()));

					node delay = nodeit.resolve("delay");
					if (delay.istype(integernode))
						delays[frame] = static_cast<short>(delay.get_integer());
					else
						delays[frame] = 50;

					textures[frame] = texture(nodeit);

					node a0_node = nodeit.resolve("a0");
					if (a0_node.istype(integernode))
					{
						byte a0 = static_cast<byte>(a0_node.get_integer());

						node a1_node = nodeit.resolve("a1");
						if (a1_node.istype(integernode))
							alphablends[frame] = pair<byte, byte>(a0, static_cast<byte>(a1_node.get_integer()));
						else
							alphablends[frame] = pair<byte, byte>(a0, 255 - a0);
					}
					else
					{
						alphablends[frame] = pair<byte, byte>(255, 255);
					}
				}
			}
		}

		frame = 0;
		last_f = textures.size() - 1;
		elapsed = 0;
		alpha = static_cast<float>(alphablends[0].first);
		alphastep = (alphablends[0].second - alpha) / (delays[0] / 16);
	}

	void animation::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		textures[frame].draw(parentpos, alpha / 255);
	}

	void animation::draw(ID2D1HwndRenderTarget* target, vector2d parentpos, float alpha)
	{
		textures[frame].draw(parentpos, alpha);
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
