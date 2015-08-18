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
#include "uielement.h"
#include "nxprovider.h"
#include "maplechar.h"
#include "uibar.h"

using namespace std;
using namespace data;
using namespace gameplay;

namespace io
{
	class login : public uielement
	{
	public:
		login(nxprovider*);
		~login() {}
		void draw(ID2D1HwndRenderTarget*);
		void update();
		void buttonpressed(short);
	};

	class loginwait : public uielement
	{
	public:
		loginwait(nxprovider*);
		~loginwait() {}
	};

	class loginnotice : public uielement
	{
	private:
		char type;
	public:
		loginnotice(nxprovider*, char);
		~loginnotice() {}
		void buttonpressed(short);
	};

	class loginworld : public uielement
	{
	public:
		loginworld(nxprovider*, char, char*);
		~loginworld() {}
		void buttonpressed(short);
	};

	class logincharsel : public uielement
	{
	private:
		vector<maplestats*> stats;
		vector<maplelook> looks;
		vector<nametag> nametags;
		charset lvset;
		charset statset;
		char selected;
	public:
		logincharsel(nxprovider*, char, vector<maplechar>*);
		~logincharsel() {}
		void draw(ID2D1HwndRenderTarget*);
		void update();
		void buttonpressed(short);
	};

	class statusbar : public uielement
	{
	private:
		maplestats* stats;
		uibar exp;
		uibar hp;
		uibar mp;
		charset statset;
		charset lvset;
	public:
		statusbar(nxprovider*, maplestats*);
		~statusbar() {}
		void draw(ID2D1HwndRenderTarget*);
		void update();
		void buttonpressed(short);
		pair<vector2d, vector2d> bounds();
	};

	void drawbar(sprite*, int, int, int, vector2d);
}

