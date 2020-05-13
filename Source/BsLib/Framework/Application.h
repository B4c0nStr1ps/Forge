#pragma once

#include "NativeWindow.h"
#include <memory>

namespace Bs::Framework
{
	class Application
	{
	public:
		void Start();
		void Stop();
		void RunMainLoop();

	protected:
		std::unique_ptr<NativeWindow> m_nativeWindow;
	};
}