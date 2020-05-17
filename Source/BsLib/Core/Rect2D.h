#pragma once
#include "Types.h"

namespace Bs
{
	struct Rect2D
	{
		Rect2D() = default;

		Rect2D(const u32 x, const u32 y, const u32 width, const u32 height)
			: x(x)
			, y(y)
			, width(width)
			, height(height)
		{}

		u32 x;
		u32 y;
		u32 width;
		u32 height;
	};
}
