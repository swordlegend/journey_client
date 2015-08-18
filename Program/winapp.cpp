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
#include "winapp.h"

namespace program
{
	winapp::~winapp()
	{ 
		SafeRelease(&d2d_factory); 
	}

	long winapp::init()
	{
		long result;

		result = initfactories();

		uinterface.init(image_factory, dwrite_factory);
		uinterface.add(UI_LOGIN);

		if (result == 0)
		{
			WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = winapp::WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(LONG_PTR);
			wcex.hInstance = HINST_THISCOMPONENT;
			wcex.hbrBackground = NULL;
			wcex.lpszMenuName = NULL;
			wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
			wcex.lpszClassName = (LPCSTR)L"Journey";

			RegisterClassEx(&wcex);

			d2d_factory->GetDesktopDpi(&dpiX, &dpiY);

			window = CreateWindow(
				(LPCSTR)L"Journey",
				"Journey",
				WS_OVERLAPPED,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				static_cast<int>(ceil(816.f * dpiX / 96.f)),
				static_cast<int>(ceil(628.f * dpiY / 96.f)),
				NULL,
				NULL,
				HINST_THISCOMPONENT,
				this
				);

			if (window)
			{
				SetPriorityClass(window, REALTIME_PRIORITY_CLASS);
				SetFocus(window);

				ShowWindow(window, SW_SHOWNORMAL);
				UpdateWindow(window);
				return 0;
			}
		}

		return result;
	}

	long winapp::initfactories()
	{
		long result = 0;

		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

		if (result == 0)
		{
			result = CoCreateInstance(
				CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&image_factory)
				);

			if (result == 0)
			{
				result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory);

				if (result == 0)
				{
					result = DWriteCreateFactory(
						DWRITE_FACTORY_TYPE_SHARED,
						__uuidof(IDWriteFactory),
						reinterpret_cast<IUnknown**>(&dwrite_factory)
						);

					return 0;
				}
			}
		}

		return 1;
	}

	LRESULT CALLBACK winapp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;
		UINT width, height;

		if (message == WM_CREATE)
		{
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			winapp *app = (winapp *)pcs->lpCreateParams;

			::SetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA,
				PtrToUlong(app)
				);

			result = 1;
		}
		else
		{
			winapp *app = reinterpret_cast<winapp *>(static_cast<LONG_PTR>(
				::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
				)));

			bool wasHandled = false;

			if (app)
			{
				switch (message)
				{
				case WM_SIZE:
					width = LOWORD(lParam);
					height = HIWORD(lParam);
					result = 0;
					wasHandled = true;
					break;
				case WM_DISPLAYCHANGE:
					InvalidateRect(hwnd, NULL, FALSE);
					result = 0;
					wasHandled = true;
					break;
				case WM_PAINT:
					app->draw();
					result = 0;
					wasHandled = true;
					break;
				case WM_DESTROY:
					//exitgame = true;
					result = 1;
					wasHandled = true;
					break;
				case WM_MOUSEMOVE:
					app->getui()->sendmouse(vector2d(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
					result = 0;
					wasHandled = true;
					break;
				case WM_SETCURSOR:
					SetCursor(NULL);
					result = 1;
					wasHandled = true;
					break;
				case WM_LBUTTONDOWN:
					switch (wParam)
					{
					case MK_LBUTTON:
						app->getui()->sendmouse(12, vector2d(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
						break;
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_LBUTTONUP:
					if (wParam != MK_LBUTTON)
					{
						app->getui()->sendmouse(0, vector2d(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
					}
					result = 0;
					wasHandled = true;
					break;
				case WM_KEYDOWN:
					app->getui()->sendkey(wParam, true);
					result = 0;
					wasHandled = true;
					break;
				case WM_KEYUP:
					app->getui()->sendkey(wParam, false);
					result = 0;
					wasHandled = true;
					break;
				}
			}

			if (!wasHandled)
			{
				result = DefWindowProc(hwnd, message, wParam, lParam);
			}
		}

		return result;
	}

	void winapp::draw()
	{
		long result = 0;

		if (!d2d_rtarget)
		{
			RECT rc;
			GetClientRect(window, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
				);

			result = d2d_factory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
				D2D1::HwndRenderTargetProperties(window, size),
				&d2d_rtarget
				);

			d2d_rtarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

			if (result == 0)
			{
				d2d_rtarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::Black),
					&black_brush
					);

				d2d_rtarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::White),
					&white_brush
					);
			}

			result = d2d_rtarget->CreateLayer(NULL, &d2d_layer);
		}

		if (result == 0)
		{
			d2d_rtarget->BeginDraw();
			d2d_rtarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
			uinterface.draw(d2d_rtarget);
			d2d_rtarget->EndDraw();
			uinterface.update();
		}
		else
		{
			SafeRelease(&d2d_rtarget);
			SafeRelease(&black_brush);
			SafeRelease(&white_brush);
		}
	}
}
