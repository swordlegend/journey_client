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
#include "vector2d.h"

namespace util
{
	vector2d::vector2d()
	{
		a = 0;
		b = 0;
	}

	vector2d::vector2d(int v)
	{
		a = v;
		b = v;
	}

	vector2d::vector2d(int v1, int v2)
	{
		a = v1;
		b = v2;
	}

	vector2d::~vector2d() {}

	int vector2d::x()
	{
		return a;
	}

	int vector2d::y()
	{
		return b;
	}

	int vector2d::length()
	{
		return static_cast<int>(std::sqrt(std::pow(a, 2) + std::pow(b, 2)));
	}

	int vector2d::center()
	{
		int delta = (a > b) ? a - b : b - a;
		return static_cast<int>(delta / 2);
	}

	bool vector2d::contains(int p)
	{
		return p >= a && p <= b;
	}

	bool vector2d::straight()
	{
		return a == b;
	}

	vector2d vector2d::operator + (vector2d v)
	{
		return vector2d(a + v.x(), b + v.y());
	}

	vector2d vector2d::operator - (vector2d v)
	{
		return vector2d(a - v.x(), b - v.y());
	}

	vector2d vector2d::operator * (vector2d v)
	{
		return vector2d(a * v.x(), b * v.y());
	}

	vector2d vector2d::operator / (vector2d v)
	{
		int x = (v.x() == 0) ? 1 : v.x();
		int y = (v.y() == 0) ? 1 : v.y();
		return vector2d(a / x, b / y);
	}

	bool vector2d::operator == (vector2d v)
	{
		return a == v.x() && b == v.y();
	}

	bool vector2d::operator != (vector2d v)
	{
		return a != v.x() || b != v.y();
	}
}
