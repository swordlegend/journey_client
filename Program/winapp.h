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
#include "ui.h"
#include "imagecache.h"
#include "fontcache.h"
#include "lookfactory.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodecsdk.h>
#include <windowsx.h>
#include <math.h>

using namespace io;

namespace program
{
	class winapp
	{
	public:
		winapp() {}
		~winapp();
		long init();
		ui* getui() { return &uinterface; }
		fontcache* getfonts() { return &fonts; }
		imagecache* getimgcache() { return &imgcache; }
		lookfactory* getlookfactory() { return &lookf; }
	private:
		long initfactories();
		void draw();
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		ui uinterface;
		fontcache fonts;
		lookfactory lookf;
		imagecache imgcache;
		HWND window;
		ID2D1Factory* d2d_factory;
		IWICImagingFactory* image_factory;
		IDWriteFactory* dwrite_factory;
		ID2D1SolidColorBrush* white_brush;
		ID2D1SolidColorBrush* black_brush;
		ID2D1HwndRenderTarget* d2d_rtarget;
		ID2D1Layer* d2d_layer;
		float dpiX;
		float dpiY;
	};
}

template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

