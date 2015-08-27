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

using namespace std;

namespace gameplay
{
	enum maplestat
	{
		SKIN = 0x1,
		FACE = 0x2,
		HAIR = 0x4,
		LEVEL = 0x10,
		JOB = 0x20,
		STR = 0x40,
		DEX = 0x80,
		INT_ = 0x100,
		LUK = 0x200,
		HP = 0x400,
		MAXHP = 0x800,
		MP = 0x1000,
		MAXMP = 0x2000,
		AP = 0x4000,
		SP = 0x8000,
		EXP = 0x10000,
		FAME = 0x20000,
		MESO = 0x40000,
		PET = 0x180008,
		GACHAEXP = 0x200000
	};

	class maplestats
	{
	public:
		maplestats(int, string, bool, short, int, int, int, short, map<maplestat, short>, pair<int, char>, vector<long>);
		maplestats() {}
		~maplestats() {}
		maplestat statvalue(char s) { return statvalues[s]; }
		void setstat(maplestat s, short v) { stats[s] = v; }
		void setskin(short v) { skin = v; }
		void setface(int v) { face = v; }
		void sethair(int v) { hair = v; }
		void setexp(int v) { exp = v; }
		void setmeso(int v) { meso = v; }
		void setfame(short v) { fame = v; }
		short getstat(maplestat);
		int getid() { return id; }
		string getname() { return name; }
		pair<int, char> getspawninfo() { return spawninfo; }
		int getexp() { return exp; }
		int getmeso() { return meso; }
		int getexpneeded() { return exptable[stats[LEVEL]]; }
	private:
		maplestat* statvalues = new maplestat[20] { SKIN, FACE, HAIR, LEVEL, JOB, STR, DEX, INT_, LUK, HP, MAXHP, MP, MAXMP, AP, SP, EXP, FAME, MESO, PET, GACHAEXP};
		int* exptable = new int[201] { 1, 15, 34, 57, 92, 135, 372, 560, 840, 1242, 1144, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, 13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, 63800, 86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372, 259392, 285532, 312928, 342624, 374760, 408336, 445544, 483532, 524160, 567772, 598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917625, 967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252, 1483314, 1564600, 1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912, 2667496, 2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832, 4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534, 8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239, 13943653, 14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594, 23772654, 25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542, 40530206, 42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311, 72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174, 117809740, 124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876, 200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102, 324648562, 342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653, 524740482, 553496261, 583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660770, 995373379, 1049919840, 1107455447, 1168144006, 1232158297, 1299680571, 1370903066, 1446028554, 1525246918, 1608855764, 1697021059};
		int id;
		string name;
		bool female;
		short skin;
		int face;
		int hair;
		int basedamage;
		int exp;
		int meso;
		short fame;
		map<maplestat, short> stats;
		pair<int, char> spawninfo;
		vector<long> petids;
	};
}

