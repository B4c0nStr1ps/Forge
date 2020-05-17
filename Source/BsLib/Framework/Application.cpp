#include "Application.h"

namespace Bs::Framework
{
	void Application::Start()
	{
		WindowGeometry nativeWindowGeometry(0, 0, 1920, 1080);
		m_nativeWindow = std::make_unique<NativeWindow>("Forge", nativeWindowGeometry);
		m_nativeWindow->Activate();
	}

	void Application::Stop()
	{
		m_nativeWindow->Deactivate();
		m_nativeWindow.reset();
	}

	void Application::RunMainLoop()
	{
		MSG msg;
		while (!WasExitMainLoopRequested())
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					ExitMainLoop();
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if (!m_nativeWindow->IsActive())
			{
				ExitMainLoop();
			}
		}
	}
}