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
#include "mapinfo.h"

namespace gameplay
{
	mapinfo::mapinfo(int id, node info, vector2d walls, vector2d borders)
	{
		mapid = id;

		if (info["VRLeft"].istype(integernode))
		{
			mapwalls = vector2d(info["VRLeft"], info["VRRight"]);
			mapborders = vector2d(info["VRTop"], info["VRBottom"]);
		}
		else
		{
			mapwalls = walls;
			mapborders = borders;
		}

		string bgmpath = info["bgm"];
		bgm = "Sound\\" + bgmpath.substr(0, bgmpath.find('/')) + ".img\\" + bgmpath.substr(0, bgmpath.find('/')) + ".img\\" + bgmpath.substr(bgmpath.find('/') + 1, bgmpath.size()) + ".mp3";
		
		cloud = info["cloud"].get_bool();
		fieldlimit = static_cast<int>(info["fieldLimit"].get_integer());
		hideminimap = info["hideMinimap"].get_bool();
		mapmark = info["mapMark"].get_string();
		swim = info["swim"].get_bool();
		town = info["town"].get_bool();
	}
}
