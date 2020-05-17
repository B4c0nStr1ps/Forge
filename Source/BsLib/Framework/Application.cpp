#include "Application.h"
//#include "../Rendering/D3D11Backend.h"

namespace Bs::Framework
{
	void Application::Start()
	{
		Rendering::D3D11Backend::Settings renderingSettings {};
		renderingSettings.viewport.geometry = {0, 0, 1920, 1080};

		m_nativeWindow = std::make_unique<NativeWindow>("Forge", renderingSettings.viewport.geometry);
		m_nativeWindow->Activate();

		m_renderingBackend = std::make_unique<Rendering::D3D11Backend>();
		m_renderingBackend->Initialize(renderingSettings);

		m_renderingBackend->CreateViewport(renderingSettings.viewport, m_nativeWindow->GetHandle());
	}

	void Application::Stop()
	{
		m_renderingBackend->Shutdown();

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

			if (!WasExitMainLoopRequested())
			{
				m_renderingBackend->Draw();
			}
		}
	}
}