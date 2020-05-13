#pragma once

#include "NativeWindow.h"
#include <memory>

namespace Bs::Framework
{
	class Application
	{
	public:
		Application() = default;

		void Start();
		void Stop();
		void RunMainLoop();

		void ExitMainLoop() { m_exitMainLoopRequested = true; }
		bool WasExitMainLoopRequested() { return m_exitMainLoopRequested; }

	protected:
		bool m_exitMainLoopRequested = false;
		std::unique_ptr<NativeWindow> m_nativeWindow;
	};
}