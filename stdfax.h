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
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <utility>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <dwrite.h>
#include <wincodecsdk.h>
#include <memory>
#include <mutex>
#include <thread>

#pragma comment(lib, "Dwrite")

#define byte unsigned char

namespace nl 
{
	class node;
	class file;
	class bitmap;
	class audio;
}

namespace data
{
	enum cachemode : char
	{
		CM_LOGIN,
		CM_SYS,
		CM_MAP,
		CM_MOB
	};
}
