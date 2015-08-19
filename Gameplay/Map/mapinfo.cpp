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
	mapinfo::mapinfo(int id, nl::node infonode)
	{
		mapid = id;

		mapwalls = vector2d(infonode.resolve("VRLeft").get_integer(), infonode.resolve("VRRight").get_integer());
		mapborders = vector2d(infonode.resolve("VRTop").get_integer(), infonode.resolve("VRBottom").get_integer());
		bgm = infonode.resolve("bgm").get_string();
		cloud = infonode.resolve("cloud").get_bool();
		fieldlimit = static_cast<int>(infonode.resolve("fieldLimit").get_integer());
		hideminimap = infonode.resolve("hideMinimap").get_bool();
		mapmark = infonode.resolve("mapMark").get_string();
		swim = infonode.resolve("swim").get_bool();
		town = infonode.resolve("town").get_bool();
	}
}
