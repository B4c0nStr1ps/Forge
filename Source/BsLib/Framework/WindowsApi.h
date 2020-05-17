#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wrl.h>

namespace Bs::WinOs
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}
