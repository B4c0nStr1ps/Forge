#pragma once
#include "../Core/Core.h"
#include "WindowsApi.h"

namespace Bs::Framework
{
	struct WindowGeometry
	{
		WindowGeometry() = default;

		WindowGeometry(const u32 posX, const u32 posY, const u32 width, const u32 height)
			: posX(posX)
			, posY(posY)
			, width(width)
			, height(height)
		{}

		u32 posX;
		u32 posY;
		u32 width;
		u32 height;
	};

	class NativeWindow
	{
	public:
		NativeWindow(const String& title, const WindowGeometry& geometry);
		~NativeWindow();

		void Activate();
		void Deactivate();
		bool IsActive() const { return m_activated; }

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
