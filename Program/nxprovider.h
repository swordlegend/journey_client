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
#include "maplemap.h"
#include "sprite.h"
#include "node.h"
#include "nxfile.h"
#include "maplelook.h"
#include "npc.h"
#include "mob.h"
#include "charset.h"
#include "portal.h"
#include "footholdtree.h"

using namespace nl;
using namespace io;
using namespace std;
using namespace gameplay;

namespace data 
{
	enum dwfonts : char
	{
		DWF_LEFT,
		DWF_CENTER,
		DWF_RIGHT,
		DWF_LARGE
	};

	class nxprovider
	{
	private:
		unique_ptr<IWICImagingFactory> imagefactory;
		unique_ptr<IDWriteFactory> fontfactory;
		pair<cachemode, size_t> cachebitmap(bitmap);
		map<dwfonts, IDWriteTextFormat*> fonts;
		void initbodyinfo();
		void initfaceinfo();
		void addbodytype(char);
		void addhairstyle(int);
		void addface(int);
		void addclothes(int);
		string getitemcategory(int);
		string getequipcategory(int);
		map<char, charsprites> bodytypes;
		map<int, charsprites> faces;
		map<int, charsprites> hairstyles;
		map<int, charsprites> clothes;
		map<string, map<char, short>> facedelays;
		map<string, map<char, short>> bodydelays;
		SRWLOCK cmlock;
		cachemode cmode;
	public:
		nxprovider();
		~nxprovider();
		void init(IWICImagingFactory*, IDWriteFactory*);
		texture loadtexture(node);
		animation loadanimation(node);
		texture loaditemicon(int, bool);
		maplemap loadmap(int);
		npc loadnpc(int);
		mob loadmob(int);
		charset loadcharset(node);
		map<string, texture> loadbutton(node);
		pair<vector<texture>, vector<texture>> loadnametag(node);
		IDWriteTextFormat* getfont(dwfonts);
		void setcmode(cachemode);
		void unlock();
		void loadcharlook(maplelook*);
		void clearcache(cachemode);
	};

	extern map<string, map<char, pair<string, char>>> bodyactions;
	extern map<string, map<char, map<charlayers, map<string, vector2d>>>> bodyheadmap;

	extern map<cachemode, map<size_t, IWICBitmap*>> cache;
	IWICBitmap* getbitmap(cachemode, size_t);
}

