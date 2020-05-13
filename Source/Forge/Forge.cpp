#include <BsLib/Framework/WindowsApi.h>
#include <BsLib/Framework/Application.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
	using namespace Bs;
	using namespace Bs::Framework;

	Application app;

	app.Start();

	app.RunMainLoop();

	app.Stop();

	return 0;
}