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
#include "settings.h"
#include <fstream>

namespace program
{
	settings::settings()
	{
		fullscreen = false;
		bgmvolume = 50;
		sfxvolume = 50;
		saveacc = false;
		defaultacc = "";

		std::ifstream config;
		std::string line;
		config.open("Settings");
		if (config.is_open()) {
			while (std::getline(config, line))
			{
				switch (line[0])
				{
				case '0':
					fullscreen = std::stoi(line.substr(1)) == 1;
					break;
				case '1':
					bgmvolume = std::stoi(line.substr(1));
					if (bgmvolume > 100)
						bgmvolume = 100;
					else if (bgmvolume < 0)
						bgmvolume = 0;
					break;
				case '2':
					sfxvolume = std::stoi(line.substr(1));
					if (sfxvolume > 100)
						sfxvolume = 100;
					else if (sfxvolume < 0)
						sfxvolume = 0;
					break;
				case '3':
					saveacc = std::stoi(line.substr(1)) == 1;
					break;
				case '4':
					defaultacc = line.substr(1);
					break;
				}
			}
		}
		config.close();
	}

	settings::~settings()
	{
	}

	void settings::save(char no, std::string param)
	{

	}

	bool settings::accsaved()
	{
		return saveacc;
	}

	std::string settings::getdefaultacc()
	{
		return defaultacc;
	}
}
