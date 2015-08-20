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
#include "packethandler.h"
#include "Journey.h"

namespace net
{
	class login_result_h : public vhandler
	{
		void login_result_h::handle(packet recv)
		{
			app.getui()->remove(UI_LOGINWAIT);
			char reason = recv.readbyte();
			if (reason > 0)
			{
				switch (reason)
				{
				case 2:
					app.getui()->add(UI_LOGINNOTICE, 16);
					return;
				default:
					app.getui()->add(UI_LOGINNOTICE, reason - 1);
					return;
				}
			}
			int accid = recv.readint();
			bool female = recv.readbool();
			char gmlevel = recv.readbyte();
			string accname = recv.readascii();
			bool muted = recv.readbool();
			short pin = recv.readshort();
			app.getui()->getfield()->getaccount()->init(accid, accname, gmlevel, female, muted, pin);
			packet_c.serverlrequest();
		}
	};

	class ping_h : public vhandler
	{
		void ping_h::handle(packet recv)
		{
			packet_c.pong();
		}
	};

	class serverl_h : public vhandler
	{
		void serverl_h::handle(packet recv)
		{
			char id = recv.readbyte();
			string name = recv.readascii();
			char flag = recv.readbyte();
			string svrmsg = recv.readascii();
			char channels = recv.readbyte();
			char* chloads = new char[channels];
			for (char i = 0; i < channels; i++)
			{
				chloads[i] = recv.readbyte();
			}
			app.getui()->getfield()->getworlds()->insert(make_pair(id, world(id, name, flag, svrmsg, channels, chloads)));
			app.getui()->remove(UI_LOGIN);
			app.getui()->add(UI_WORLDSELECT);
		}
	};

	class charlist_h : public vhandler
	{
		void charlist_h::handle(packet recv)
		{
			char numchars = recv.readbyte();
			vector<maplechar> maplechars;
			for (char i = 0; i < numchars; i++)
			{
				maplechars.push_back(getmchar(&recv));
				bool viewall = recv.readbool();
				bool gm = recv.readbool();
				if (gm)
				{
					continue;
				}
				bool ranked = recv.readbool();
				int rank = recv.readint();
				int rankmv = recv.readint();
				int jobrank = recv.readint();
				int jobrankmv = recv.readint();
			}
			char pic = recv.readbyte();
			char slots = recv.readbyte();
			app.getui()->getfield()->getaccount()->addchars(maplechars, pic, slots);
			app.getui()->remove(UI_WORLDSELECT);
			app.getui()->add(UI_CHARSEL);
		}

		maplechar getmchar(packet* recv)
		{
			maplestats mstats = getmstats(recv);
			maplelook mlook = getmlook(recv);
			return maplechar(mstats, mlook);
		}

		maplestats getmstats(packet* recv)
		{
			int id = recv->readint();
			string name = recv->readpadascii(13);
			bool female = recv->readbool();
			char skin = recv->readbyte();
			int face = recv->readint();
			int hair = recv->readint();
			vector<long> pets;
			for (char i = 0; i < 3; i++)
			{
				pets.push_back(recv->readlong());
			}
			short level = recv->readshort();
			short job = recv->readshort();
			short str = recv->readshort();
			short dex = recv->readshort();
			short int_ = recv->readshort();
			short luk = recv->readshort();
			short hp = recv->readshort();
			short mhp = recv->readshort();
			short mp = recv->readshort();
			short mmp = recv->readshort();
			short ap = recv->readshort();
			short sp = recv->readshort();
			int exp = recv->readint();
			short fame = recv->readshort();
			int map = recv->readint();
			char spawnp = recv->readbyte();
			return maplestats(id, name, female, skin, face, hair, pets, level, job, str, dex, int_, luk, hp, mhp, mp, mmp, ap, sp, exp, fame, map, spawnp);
		}

		maplelook getmlook(packet* recv)
		{
			bool female = recv->readbool();
			char skin = recv->readbyte();
			int face = recv->readint();
			recv->readbyte();
			int hair = recv->readint();

			map<char, int> equips;
			byte slot = recv->readbyte();
			while (slot != 0xFF)
			{
				equips[slot] = recv->readint();
				slot = recv->readbyte();
			}

			map<char, int> mskequips;
			slot = recv->readbyte();
			while (slot != 0xFF)
			{
				mskequips[slot] = recv->readint();
				slot = recv->readbyte();
			}

			mskequips[-111] = recv->readint();

			vector<int> petids;
			for (char i = 0; i < 3; i++)
			{
				petids.push_back(recv->readint());
			}

			return maplelook(female, skin, face, hair, equips, mskequips, petids);
		}
	}; 
	
	class delchar_response_h : public vhandler
	{
		void delchar_response_h::handle(packet recv)
		{
			int cid = recv.readint();
			bool success = recv.readbool();
			if (success)
			{
				app.getui()->add(io::UI_LOGINNOTICE, 55);
			}
			else
			{
				app.getui()->add(io::UI_LOGINNOTICE, 93);
			}
		}
	};

	class server_ip_h : public vhandler
	{
		void server_ip_h::handle(packet recv)
		{
			char* addr = new char[4];
			for (char i = 0; i < 4; i++)
			{
				addr[i] = recv.readbyte();
			}
			short port = recv.readshort();
			int clientid = recv.readint();
			server.reconnect(addr, port);
			packet_c.playerlogin(clientid);
		}
	};

	class server_message_h : public vhandler
	{
		void server_message_h::handle(packet recv)
		{
			char type = recv.readbyte();
			bool server = recv.readbool();
			string msg = recv.readascii();
			if (type == 3)
			{
				char channel = recv.readbyte() + 1;
				bool mega = recv.readbool();
			}
			//to do
		}
	};

	class charinfo_h : public vhandler
	{
		void charinfo_h::handle(packet recv)
		{
			recv.readint();
			recv.readint();
			recv.readint();
			recv.readint();
			recv.readint();
			recv.readlong();
			recv.readbyte();
			char buddycap = recv.readbyte();
			if (recv.readbool())
			{
				string lname = recv.readascii();
			}
			int meso = recv.readint();

			inventory inv = getinventory(&recv);
			recv.readbyte();

			map<int, pair<pair<int, int>, long>> skills;
			short size = recv.readshort();
			int skillid, level, masterlevel;
			long expire;
			for (short i = 0; i < size; i++)
			{
				skillid = recv.readint();
				level = recv.readint();
				expire = recv.readlong();
				bool fourthjob = recv.readbool();
				if (fourthjob)
				{
					masterlevel = recv.readint();
				}
				else
				{
					masterlevel = -1;
				}
				skills.insert(make_pair(skillid, pair<pair<int, int>, long>(pair<int, int>(level, masterlevel), expire)));
			}
			map<int, short> cooldowns;
			size = recv.readshort();
			short cdtime;
			for (short i = 0; i < size; i++)
			{
				skillid = recv.readint();
				cdtime = recv.readint();
				cooldowns.insert(make_pair(skillid, cdtime));
			}

			map<int, pair<string, pair<short, string>>> quests;
			size = recv.readshort();
			short qid;
			string qdata;
			short infon;
			string qprogress;
			for (short i = 0; i < size; i++)
			{
				qid = recv.readshort();
				qdata = recv.readascii();
				bool info = recv.readbool();
				if (info)
				{
					infon = recv.readshort();
					qprogress = recv.readascii();
				}
				else
				{
					infon = -1;
					qprogress = "";
				}
				quests.insert(make_pair(qid, pair<string, pair<short, string>>(qdata, pair<short, string>(infon, qprogress))));
			}
			map<int, long> completedquests;
			size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				qid = recv.readshort();
				expire = recv.readlong();
				completedquests.insert(make_pair(qid, expire));
			}

			pair<vector<int>, vector<int>> trockmaps;
			for (char i = 0; i < 15; i++)
			{
				if (i < 5)
					trockmaps.first.push_back(recv.readint());
				else
					trockmaps.second.push_back(recv.readint());
			}

			int bookcover = recv.readint();
			recv.readbyte();
			map<short, char> bookcards;
			size = recv.readshort();
			char mblv;
			for (short i = 0; i < size; i++)
			{
				qid = recv.readshort();
				mblv = recv.readbyte();
				bookcards.insert(make_pair(qid, mblv));
			}

			map<short, string> areainfo;
			size = recv.readshort();
			for (short i = 0; i < size; i++)
			{
				qid = recv.readshort();
				qdata = recv.readascii();
				areainfo.insert(make_pair(qid, qdata));
			}

			long timestamp = recv.readlong();
			app.getui()->remove(UI_CHARSEL);
			app.getui()->getprovider()->clearcache(CM_LOGIN);
			app.getui()->add(UI_STATUSBAR);
			app.getui()->getfield()->setplayer(player(app.getui()->getfield()->getaccount()->getplayer(), inv, skills, cooldowns, quests, completedquests, trockmaps, bookcover, bookcards, areainfo));
			app.getui()->getfield()->setfield(app.getui()->getprovider(), app.getui()->getfield()->getplayer()->getstats()->map, app.getui()->getfield()->getplayer()->getstats()->spawnp);
			app.getui()->enableactions();
		}

		inventory charinfo_h::getinventory(packet* recv)
		{
			vector<char> slots;
			for (char i = 0; i < 5; i++)
			{
				slots.push_back(recv->readbyte());
			}
			recv->readlong();
			map<short, mapleequip> equipped;
			map<short, mapleequip> equippedcash;
			map<short, mapleequip> equips;
			for (char i = 0; i < 3; i++)
			{
				int endofeq = recv->readint();
				while (endofeq != 0)
				{
					short pos = recv->readshort();
					char type = recv->readbyte();
					int id = recv->readint();
					bool cash = recv->readbool();
					long uniqueid = -1;
					if (cash)
					{
						uniqueid = recv->readlong();
						recv->readbyte();
					}
					long expire = recv->readlong();
					char slots = recv->readbyte();
					char level = recv->readbyte();
					map<equipstat, short> stats;
					for (equipstat e = ES_STR; e <= ES_JUMP; e = (equipstat)(e + 1))
					{
						stats[e] = recv->readshort();
					}
					string owner = recv->readascii();
					short flag = recv->readshort();
					char itemlevel = 0;
					short itemexp = 0;
					int vicious = 0;
					if (cash)
					{
						recv->readlong();
						recv->readshort();
					}
					else 
					{
						recv->readbyte();
						itemlevel = recv->readbyte();
						recv->readshort();
						itemexp = recv->readshort();
						vicious = recv->readint();
						recv->readlong();
					}
					recv->readlong();
					endofeq = recv->readint();
					mapleequip equip = mapleequip(pos, type, id, cash, uniqueid, expire, slots, level, stats, owner, flag, itemlevel, itemexp, vicious);
					switch (i)
					{
					case 0:
						equipped.insert(make_pair(pos, equip));
						break;
					case 1:
						equippedcash.insert(make_pair(pos, equip));
						break;
					case 2:
						equips.insert(make_pair(pos, equip));
						break;
					}
				}
			}
			map<char, mapleitem> useitems;
			map<char, mapleitem> setupitems;
			map<char, mapleitem> etcitems;
			map<char, mapleitem> cashitems;
			for (char i = 0; i < 4; i++)
			{
				char endoflist = recv->readbyte();
				while (endoflist != 0)
				{
					char pos = recv->readbyte();
					char type = recv->readbyte();
					int id = recv->readint();
					bool cash = recv->readbool();
					long uniqueid = -1;
					bool pet = false;
					if (cash)
					{
						uniqueid = recv->readlong();
						pet = recv->readbool();
					}
					long expire = recv->readlong();
					string petname;
					char petlevel;
					short closeness;
					char fullness;
					if (pet)
					{
						petname = recv->readpadascii(13);
						petlevel = recv->readbyte();
						closeness = recv->readshort();
						fullness = recv->readbyte();
						mapleitem item = mapleitem(id, pos, type, cash, uniqueid, expire, petname, petlevel, closeness, fullness);
						endoflist = recv->readbyte();
						cashitems.insert(make_pair(pos, item));
						continue;
					}
					short count = recv->readshort();
					string owner = recv->readascii();
					short flag = recv->readshort();
					endoflist = recv->readbyte();
					mapleitem item = mapleitem(id, count, pos, type, cash, uniqueid, expire, owner, flag);
					switch (i)
					{
					case 0:
						useitems.insert(make_pair(pos, item));
						break;
					case 1:
						setupitems.insert(make_pair(pos, item));
						break;
					case 2:
						etcitems.insert(make_pair(pos, item));
						break;
					case 3:
						cashitems.insert(make_pair(pos, item));
						break;
					}
				}
			}
			return inventory(slots, equipped, equippedcash, equips, useitems, setupitems, etcitems, cashitems);
		}
	};

	class keymap_h : public vhandler
	{
		void keymap_h::handle(packet recv)
		{
			recv.readbyte();
			map<char, pair<char, int>> keymap;
			for (char i = 0; i < 90; i++)
			{
				char type = recv.readbyte();
				int action = recv.readint();
				keymap[i] = make_pair(type, action);
			}
			app.getui()->getkeyboard()->init(keymap);
		}
	};

	class skill_macros_h : public vhandler
	{
		void skill_macros_h::handle(packet recv)
		{
			map<string, pair<char, vector<int>>> macros;
			char size = recv.readbyte();
			for (char i = 0; i < size; i++)
			{
				string name = recv.readascii();
				char shout = recv.readbyte();
				vector<int> skills;
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				skills.push_back(recv.readint());
				macros.insert(make_pair(name, make_pair(shout, skills)));
			}
			app.getui()->getkeyboard()->addmacros(macros);
		}
	};

	class unhandled : public vhandler
	{
		void unhandled::handle(packet recv)
		{
			//To Do
		}
	};

	class spawn_npc_h : public vhandler
	{
		void spawn_npc_h::handle(packet recv)
		{
			int oid = recv.readint();
			int id = recv.readint();
			short posx = recv.readshort();
			short cy = recv.readshort();
			bool f = recv.readbool();
			short fh = recv.readshort();
			short rx = recv.readshort();
			short ry = recv.readshort();
			npc toadd = app.getui()->getprovider()->loadnpc(id);
			toadd.setinfo(oid, f, fh, vector2d(posx, cy));
			app.getui()->getfield()->getmapobjects()->addnpc(oid, toadd);
		}
	};

	class buddylist_h : public vhandler
	{
		void buddylist_h::handle(packet recv)
		{
			//To Do
		}
	};

	class family_plist_h : public vhandler
	{
		void family_plist_h::handle(packet recv)
		{
			//To Do
		}
	};

	class memo_result_h : public vhandler
	{
		void memo_result_h::handle(packet recv)
		{
			//To Do
		}
	};

	class enable_report_h : public vhandler
	{
		void enable_report_h::handle(packet recv)
		{
			bool report = recv.readbool();
			//do whatever
		}
	};

	class update_skills_h : public vhandler
	{
		void update_skills_h::handle(packet recv)
		{
			recv.readbyte();
			recv.readshort();
			int skillid = recv.readint();
			int level = recv.readint();
			int masterlevel = recv.readint();
			long expire = recv.readlong();
			app.getui()->getfield()->getplayer()->updateskill(skillid, level, masterlevel, expire);
		}
	};

	class week_event_messsage_h : public vhandler
	{
		void week_event_messsage_h::handle(packet recv)
		{
			//To do
		}
	};

	class modify_inventory_h : public vhandler
	{
		void modify_inventory_h::handle(packet recv)
		{
			bool updatetick = recv.readbool();
			char size = recv.readbyte();
			for (char i = 0; i < size; i++)
			{
				char mode = recv.readbyte();
				char invtype = recv.readbyte();
				short pos = recv.readshort(); //if move: oldposition, else position
				short arg = recv.readshort();

				if ((mode == 2 && (pos < 0 || arg < 0)) || mode == 3 && pos < 0)
				{
					char addmove = recv.readbyte();
				}

				switch (mode)
				{
				case 0:
					//additem
					break;
				case 1:
					//quantity
					break;
				case 2:
					//move
					break;
				case 3:
					//remove
					break;
				}
			}
		}
	};

	class spawn_mob_h : public vhandler
	{
		void spawn_mob_h::handle(packet recv)
		{
			int oid = recv.readint();
			bool control = recv.readbool();
			int id = recv.readint();
			vector2d pos = recv.readpoint();
			char stance = recv.readbyte();
			short fh = recv.readshort();
			bool haseffect = recv.readbool();
			char effect = 0;
			if (haseffect)
			{
				effect = recv.readbyte();
			}
			bool fadein = recv.readbool();
			char team = recv.readbyte();
			mob toadd = app.getui()->getprovider()->loadmob(id);
			toadd.setinfo(oid, pos, pos, stance, fh, effect, fadein, team);
			app.getui()->getfield()->getmapobjects()->addmob(oid, toadd);
		}
	};

	class spawn_mob_c_h : public vhandler
	{
		void spawn_mob_c_h::handle(packet recv)
		{
			char aggro = recv.readbyte();
			int oid = recv.readint();
			if (aggro == 0)
			{
				return;
			}
			bool control = recv.readbool();
			int id = recv.readint();
			vector2d pos = recv.readpoint();
			char stance = recv.readbyte();
			short fh = recv.readshort();
			bool haseffect = recv.readbool();
			char effect = 0;
			if (haseffect)
			{
				effect = recv.readbyte();
			}
			bool fadein = recv.readbool();
			char team = recv.readbyte();
			mob toadd = app.getui()->getprovider()->loadmob(id);
			toadd.setinfo(oid, pos, pos, stance, fh, effect, fadein, team);
			app.getui()->getfield()->getmapobjects()->addmob(oid, toadd);
		}
	};

	class stats_changed_h : public vhandler
	{
		void stats_changed_h::handle(packet recv)
		{
			bool itemreaction = recv.readbool();
			int updatemask = recv.readint();
			if (updatemask > 0)
			{

			}
			else
			{
				app.getui()->enableactions();
			}
		}
	};

	class warp_to_map_h : public vhandler
	{
		void warp_to_map_h::handle(packet recv)
		{
			char channel = recv.readbyte();
			int mapid = recv.readint();
			char portalid = recv.readbyte();
			long timestamp = recv.readlong();
			if (channel == app.getui()->getfield()->getchannel())
			{
				app.getui()->getprovider()->clearcache(CM_MAP);
				app.getui()->getfield()->setfield(app.getui()->getprovider(), mapid, portalid);
			}
			else
			{
				app.getui()->getfield()->changechannel(channel);
			}
			app.getui()->enableactions();
		}
	};

	class clock_h : public vhandler
	{
		void clock_h::handle(packet recv)
		{
			char ctype = recv.readbyte();
			if (ctype == 1)
			{
				char hour = recv.readbyte();
				char minute = recv.readbyte();
				char second = recv.readbyte();
			}
			else if (ctype == 2)
			{
				int time = recv.readint();
			}
			//TO DO
		}
	};

	class spawn_reactor_h : public vhandler
	{
		void spawn_reactor_h::handle(packet recv)
		{
			int oid = recv.readint();
			int id = recv.readint();
			char state = recv.readbyte();
			vector2d pos = recv.readpoint();
			//app.getui()->getfield()->getmap()->addreactor(oid, id, state, pos);
		}
	};

	class show_mob_hp_h : public vhandler
	{
		void show_mob_hp_h::handle(packet recv)
		{
			int oid = recv.readint();
			char hppercent = recv.readbyte();
			app.getui()->getfield()->getmapobjects()->sendmobhp(oid, hppercent);
		}
	};

	class kill_mob_h : public vhandler
	{
		void kill_mob_h::handle(packet recv)
		{
			int oid = recv.readint();
			int animation = recv.readint();
			app.getui()->getfield()->getmapobjects()->killmob(oid, animation);
		}
	};

	class drop_item_from_mapobject_h : public vhandler
	{
		void drop_item_from_mapobject_h::handle(packet recv)
		{
			char mod = recv.readbyte();
			int oid = recv.readint();
			bool meso = recv.readbool();
			int itemid = recv.readint();
			int owner = recv.readint();
			char pickuptype = recv.readbyte();
			vector2d dropto = recv.readpoint();
			vector2d dropfrom;
			recv.readint();
			if (mod != 2)
			{
				dropfrom = recv.readpoint();
				recv.readshort();
			}
			else
			{
				dropfrom = dropto;
			}

			if (!meso)
			{
				long expire = recv.readlong();
			}
			bool playerdrop = recv.readbool();
			
			itemdrop drop = itemdrop(app.getui()->getprovider()->loaditemicon(itemid, meso), dropfrom, dropto, oid, itemid, owner, meso, pickuptype, playerdrop);
			app.getui()->getfield()->getmapobjects()->additemdrop(oid, drop);
		}
	};

	class remove_mapitem_h : public vhandler
	{
		void remove_mapitem_h::handle(packet recv)
		{
			char anim = recv.readbyte();
			int oid = recv.readint();
			if (anim > 1)
			{
				int cid = recv.readint();
			}
			app.getui()->getfield()->getmapobjects()->removedrop(oid);
		}
	};

	packethandler::packethandler()
	{
		handlers.insert(make_pair(LOGIN_RESULT, unique_ptr<vhandler>(new login_result_h())));
		handlers.insert(make_pair(SERVERLIST, unique_ptr<vhandler>(new serverl_h())));
		handlers.insert(make_pair(CHARLIST, unique_ptr<vhandler>(new charlist_h())));
		handlers.insert(make_pair(DELCHAR_RESPONSE, unique_ptr<vhandler>(new delchar_response_h())));
		handlers.insert(make_pair(SERVER_IP, unique_ptr<vhandler>(new server_ip_h())));
		handlers.insert(make_pair(BUDDY_LIST, unique_ptr<vhandler>(new buddylist_h())));
		handlers.insert(make_pair(SERVER_MESSAGE, unique_ptr<vhandler>(new server_message_h())));
		handlers.insert(make_pair(WEEK_EVENT_MESSAGE, unique_ptr<vhandler>(new week_event_messsage_h())));
		handlers.insert(make_pair(PING, unique_ptr<vhandler>(new ping_h())));
		handlers.insert(make_pair(SHOW_STATUS_INFO, unique_ptr<vhandler>(new unhandled())));
		handlers.insert(make_pair(WARP_TO_MAP, unique_ptr<vhandler>(new warp_to_map_h())));
		handlers.insert(make_pair(MODIFY_INVENTORY, unique_ptr<vhandler>(new modify_inventory_h())));
		handlers.insert(make_pair(STATS_CHANGED, unique_ptr<vhandler>(new stats_changed_h())));
		handlers.insert(make_pair(UPDATE_SKILLS, unique_ptr<vhandler>(new update_skills_h())));
		handlers.insert(make_pair(MEMO_RESULT, unique_ptr<vhandler>(new memo_result_h())));
		handlers.insert(make_pair(ENABLE_REPORT, unique_ptr<vhandler>(new enable_report_h())));
		handlers.insert(make_pair(SCRIPT_PROGRESS_MESSAGE, unique_ptr<vhandler>(new unhandled())));
		handlers.insert(make_pair(SKILL_MACROS, unique_ptr<vhandler>(new skill_macros_h())));
		handlers.insert(make_pair(FAMILY_PRIV_LIST, unique_ptr<vhandler>(new family_plist_h())));
		handlers.insert(make_pair(CHARINFO, unique_ptr<vhandler>(new charinfo_h())));
		handlers.insert(make_pair(SPAWN_MOB, unique_ptr<vhandler>(new spawn_mob_h())));
		handlers.insert(make_pair(KILL_MOB, unique_ptr<vhandler>(new kill_mob_h)));
		handlers.insert(make_pair(SPAWN_MOB_C, unique_ptr<vhandler>(new spawn_mob_c_h())));
		handlers.insert(make_pair(SHOW_MOB_HP, unique_ptr<vhandler>(new show_mob_hp_h)));
		handlers.insert(make_pair(SPAWN_NPC, unique_ptr<vhandler>(new spawn_npc_h())));
		handlers.insert(make_pair(DROP_ITEM_FROMOBJECT, unique_ptr<vhandler>(new drop_item_from_mapobject_h())));
		handlers.insert(make_pair(REMOVE_MAPITEM, unique_ptr<vhandler>(new remove_mapitem_h())));
		handlers.insert(make_pair(KEYMAP, unique_ptr<vhandler>(new keymap_h())));
		handlers.insert(make_pair(CLOCK, unique_ptr<vhandler>(new clock_h())));
		handlers.insert(make_pair(SPAWN_PET, unique_ptr<vhandler>(new unhandled())));
		handlers.insert(make_pair(SPAWN_REACTOR, unique_ptr<vhandler>(new spawn_reactor_h())));
		handlers.insert(make_pair(FORCED_STAT_RESET, unique_ptr<vhandler>(new unhandled())));
	}

	packethandler::~packethandler() 
	{
		for (map<short, unique_ptr<vhandler>>::iterator hit = handlers.begin(); hit != handlers.end(); hit++)
		{
			hit->second.release();
		}
	}

	void packethandler::process(packet recv)
	{
		short opcode = recv.readshort();
		handlers[opcode]->handle(recv);
	}
}
