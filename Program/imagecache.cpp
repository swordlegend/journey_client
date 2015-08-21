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
#include "imagecache.h"

namespace program
{
	void imagecache::init(IWICImagingFactory* imf)
	{
		imgfactory = unique_ptr<IWICImagingFactory>(imf);
		target = 0;
		imgcon = ict_login;
	}

	pair<imgcontext, size_t> imagecache::createimage(bitmap bmp)
	{
		size_t id = bmp.id();

		if (temp[imgcon][id] || cache[imgcon][id])
		{
			return make_pair(imgcon, id);
		}
		else
		{
			IWICBitmap* wic = 0;

			imgfactory->CreateBitmapFromMemory(
				bmp.width(), bmp.height(),
				GUID_WICPixelFormat32bppBGRA,
				4 * bmp.width(),
				bmp.length(),
				(BYTE*)bmp.data(),
				&wic);

			if (wic)
			{
				IWICFormatConverter* spConverter = 0;
				int result = imgfactory->CreateFormatConverter(&spConverter);
				if (result == 0)
				{
					spConverter->Initialize(wic, GUID_WICPixelFormat32bppPBGRA,
						WICBitmapDitherTypeNone, NULL, 0.f,
						WICBitmapPaletteTypeMedianCut);
					imgfactory->CreateBitmapFromSource(spConverter, WICBitmapNoCache, &temp[imgcon][id]);
					spConverter->Release();
				}
				wic->Release();
			}
		}

		return make_pair(imgcon, id);
	}

	void imagecache::draw(imgcontext ict, size_t id, D2D1_RECT_F r, float alpha)
	{
		if (!cache[ict][id])
		{
			target->CreateBitmapFromWicBitmap(temp[ict][id], &cache[ict][id]);
			temp[ict][id]->Release();
			temp[ict].erase(id);

			if (cache[ict][id] == 0)
			{
				cache[ict].erase(id);
			}
		}

		if (cache[ict][id])
		{
			target->DrawBitmap(cache[ict][id], r, alpha);
		}
	}

	void imagecache::setmode(imgcontext ict)
	{
		modelock.lock();
		imgcon = ict;
	}

	void imagecache::unlock()
	{
		modelock.unlock();
	}

	void imagecache::clearcache(imgcontext ict)
	{
		for (map<size_t, IWICBitmap*>::iterator cit = temp[ict].begin(); cit != temp[ict].end(); ++cit)
		{
			if (cit->second != 0)
			{
				cit->second->Release();
				cit->second = 0;
			}
		}
		temp[ict].clear();

		for (map<size_t, ID2D1Bitmap*>::iterator cit = cache[ict].begin(); cit != cache[ict].end(); ++cit)
		{
			if (cit->second != 0)
			{
				cit->second->Release();
				cit->second = 0;
			}
		}
		cache[ict].clear();
	}
}
