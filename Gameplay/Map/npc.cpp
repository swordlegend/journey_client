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
#include "npc.h"
#include "Journey.h"
#include "nxfile.h"

namespace gameplay
{
	npc::npc(int id, int o, bool fr, short f, vector2d pos)
	{
		app.getimgcache()->setmode(ict_map);
		nx::view_file("Npc");

		string fullname;
		string strid = to_string(id);
		char extend = 7 - strid.length();
		for (char i = 0; i < extend; i++)
		{
			fullname.append("0");
		}
		fullname.append(strid);
		node npcdata = nx::nodes["Npc"].resolve(fullname + ".img");

		map<string, vector<string>> linenames;
		for (node npcnode = npcdata.begin(); npcnode != npcdata.end(); npcnode++)
		{
			string state = npcnode.name();
			if (state == "info")
			{
				node speak = npcnode.resolve("speak");
				if (speak.size() > 0)
				{
					for (node speaknode = speak.begin(); speaknode != speak.end(); speaknode++)
					{
						linenames[state].push_back(speaknode.get_string());
					}
				}
			}
			else
			{
				textures[state] = animation(npcnode);
				node speak = npcnode.resolve("speak");
				if (speak.size() > 0)
				{
					for (node speaknode = speak.begin(); speaknode != speak.end(); speaknode++)
					{
						linenames[state].push_back(speaknode.get_string());
					}
				}
			}
		}

		nx::unview_file("Npc");
		nx::view_file("String");

		node stringdata = nx::nodes["String"].resolve("Npc.img/" + strid);

		node namenode = stringdata.resolve("name");
		if (namenode.data_type() == node::type::string)
			name = namenode.get_string();

		node funcnode = stringdata.resolve("func");
		if (funcnode.data_type() == node::type::string)
			func = funcnode.get_string();

		for (map<string, vector<string>>::iterator stit = linenames.begin(); stit != linenames.end(); stit++)
		{
			string state = stit->first;
			vector<string> names = stit->second;
			for (vector<string>::iterator nit = names.begin(); nit != names.end(); nit++)
			{
				string line = stringdata.resolve(*nit).get_string();
				lines[state].push_back(line);
			}
		}

		nx::unview_file("String");
		app.getimgcache()->unlock();

		oid = o;
		front = fr;
		fh = f;
		position = pos;
		state = "stand";
	}

	void npc::draw(ID2D1HwndRenderTarget* target, vector2d parentpos)
	{
		textures[state].draw(target, parentpos + position);
	}

	void npc::update()
	{
		textures[state].update();
	}
}
