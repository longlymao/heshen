#include "mainapp.h"

static int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow)
{
	rrender::MainApp app;
    app.Init();
    app.MainLoop();
    return 0;
}
