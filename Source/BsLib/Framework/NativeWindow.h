#pragma once
#include "../Core/Core.h"
#include "../Core/Rect2D.h"
#include "WindowsApi.h"

namespace Bs::Framework
{
	class NativeWindow
	{
	public:
		NativeWindow(const String& title, const Rect2D& geometry);
		~NativeWindow();

		void Activate();
		void Deactivate();
		bool IsActive() const { return m_activated; }
		HWND GetHandle() const { return m_win32Handle; }

	protected:
		static LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void WindowSizeChanged(u32 width, u32 height);

	protected:
		HWND m_win32Handle;
		u32 m_width;
		u32 m_height;
		bool m_activated;
	};
}
