#pragma once

#include "../Core/Core.h"
#include "../Core/Rect2D.h"
#include "D3D11Api.h"

namespace Bs::Rendering
{
	class D3D11Viewport
	{
	public:
		D3D11Viewport(Rect2D geometry);
	};
}