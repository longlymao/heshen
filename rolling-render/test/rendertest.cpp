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

	auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2) {
		rrender::Line* line = new rrender::Line();
		line->SetPos1(rmath::VectorF3(x1, y1, z1));
		line->SetPos2(rmath::VectorF3(x2, y2, z2));
		world->AddLine(line);
		};

	AddLine(0, 0, 0, 200, 0, 0);
	AddLine(200, 0, 0, 200, 200, 0);
	AddLine(200, 200, 0, 0, 200, 0);
	AddLine(0, 200, 0, 0, 0, 0);

	auto AddLine2 = [&](int x1, int y1, int z1, int x2, int y2, int z2) {
		AddLine(x1 + 50, y1 + 50, z1, x2 + 50, y2 + 50, z2);
		};

	AddLine2(0, 0, 200, 200, 0, 200);
	AddLine2(200, 0, 200, 200, 200, 200);
	AddLine2(200, 200, 200, 0, 200, 200);
	AddLine2(0, 200, 200, 0, 0, 200);

	auto AddLine3 = [&](int x1, int y1, int z1, int x2, int y2, int z2) {
		AddLine(x1, y1, z1, x2 + 50, y2 + 50, z2);
		};
	
	AddLine3(0, 0, 0, 0, 0, 200);
	AddLine3(200, 0, 0, 200, 0, 200);
	AddLine3(200, 200, 0, 200, 200, 200);
	AddLine3(0, 200, 0, 0, 200, 200);
	

	//AddLine(000, 100, 200, 200);
	//AddLine(400, 300, 200, 200);
	//AddLine(400, 100, 200, 200);
	//AddLine(000, 300, 200, 200);

	//AddLine(100, 000, 200, 200);
	//AddLine(300, 400, 200, 200);
	//AddLine(100, 400, 200, 200);
	//AddLine(300, 000, 200, 200);

	//AddLine(0, 200, 400, 200);
	//AddLine(200, 400, 200, 0);
	//AddLine(0, 0, 400, 400);
	//AddLine(0, 400, 400, 0);

	app.GetScene().AddWorld(world);

	app.MainLoop();
	return 0;
}
