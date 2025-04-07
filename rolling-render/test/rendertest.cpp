#include "renderapp.h"

static int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	rrender::RenderApp app;
	app.Init();
	app.MainLoop();
	return 0;
}
