#include "NativeWindow.h"

namespace Bs::Framework
{
	const char* s_defaultClassName = "BsFrameworkWin32Class";

	NativeWindow::NativeWindow(const String& title, const WindowGeometry& geometry)
		: m_activated(false)
		, m_width(geometry.width)
		, m_height(geometry.height)
		, m_win32Handle(nullptr)
	{
		const HINSTANCE hInstance = GetModuleHandle(0);
		// register window class if it does not exist
		WNDCLASSEX windowClass;
		if (GetClassInfoEx(hInstance, s_defaultClassName, &windowClass) == false)
		{
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			windowClass.lpfnWndProc = &NativeWindow::WindowCallback;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = hInstance;
			windowClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = s_defaultClassName;
			windowClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

			if (!RegisterClassEx(&windowClass))
			{
				// TODO error logging.
				//AZ_Error("Windowing", false, "Failed to register Win32 window class with error: %d", GetLastError());
			}

			// These will change from the external size to the client area size once we receive a WM_Size message
			m_width = geometry.width;
			m_height = geometry.height;

			// create main window
			m_win32Handle = CreateWindow(
				s_defaultClassName, title.c_str(),
				WS_OVERLAPPEDWINDOW,
				geometry.posX, geometry.posY, geometry.width, geometry.height,
				NULL, NULL, hInstance, NULL);

			if (m_win32Handle == nullptr)
			{
				// TODO error logging.
				//AZ_Error("Windowing", false, "Failed to create Win32 window with error: %d", GetLastError());
			}
			else
			{
				SetWindowLongPtr(m_win32Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			}
		}
	}

	NativeWindow::~NativeWindow()
	{
		Deactivate();
	}

	void NativeWindow::Activate()
	{
		if (!m_activated)
		{
			m_activated = true;

			// This will result in a WM_SIZE message which will send the OnWindowResized notification
			ShowWindow(m_win32Handle, SW_SHOW);
			UpdateWindow(m_win32Handle);
		}
	}

	void NativeWindow::Deactivate()
	{
		if (m_activated) // nothing to do if window was already deactivated
		{
			m_activated = false;

			ShowWindow(m_win32Handle, SW_HIDE);
			UpdateWindow(m_win32Handle);
		}
	}

	LRESULT NativeWindow::WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		NativeWindow* nativeWindowImpl = reinterpret_cast<NativeWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (message)
		{
		case WM_CLOSE:
		{
			nativeWindowImpl->Deactivate();
			break;
		}
		case WM_SIZE:
		{
			const uint16_t newWidth = LOWORD(lParam);
			const uint16_t newHeight = HIWORD(lParam);

			nativeWindowImpl->WindowSizeChanged(static_cast<uint32_t>(newWidth), static_cast<uint32_t>(newHeight));
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
	void NativeWindow::WindowSizeChanged(u32 width, u32 height)
	{
	}
}