#include "renderapp.h"

#include "matrix.h"

static int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	rrender::RenderApp app;
	app.Init();
	app.SetFrameRate(60);

	rrender::World2D* world = new rrender::World2D();

	auto AddLine = [&](int x1, int y1, int x2, int y2) {
		rrender::Line* line = new rrender::Line();
		line->SetPos1(rmath::VectorF3(x1, y1, 0));
		line->SetPos2(rmath::VectorF3(x2, y2, 0));
		world->AddLine(line);
		};

	AddLine(100, 100, 200, 200);
	AddLine(300, 300, 200, 200);
	AddLine(300, 100, 200, 200);
	AddLine(100, 300, 200, 200);

	app.GetScene().AddWorld(world);

	app.MainLoop();
	return 0;
}
