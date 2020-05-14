#pragma once

#include "..\Core\Core.h"
#include "NativeWindow.h"

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
		UniquePtr<NativeWindow> m_nativeWindow;
	};
}