#include "renderapp.h"

#include "base/color.h"

#include "ref.h"

#include "scene/node/line2d.h"
#include "scene/node/cube.h"

static int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	rolling::RenderApp app;
	app.Init();
	app.SetFrameRate(60);

	rolling::World2D* world = new rolling::World2D();
	app.GetScene().AddWorld(world);

	auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2, 
		rolling::Color color1 = rolling::Color::WHITE, rolling::Color color2 = rolling::Color::WHITE) {

		rolling::base::SharedPtr<rolling::Line2D> line = rolling::Line2D::create();
		line->SetPosition(300, 200);
		line->SetPos1(x1, y1);
		line->SetPos2(x2, y2);
		world->AddNode(line);
		line->SetColor1(color1);
		line->SetColor2(color2);

		//rolling::SharedPtr<rolling::Line2D> line2 = rolling::Line2D::create();
		//line2->SetPosition(10, 10);
		//line2->SetScale(2, 2);
		//line2->SetPos1(x1, y1);
		//line2->SetPos2(x2, y2);
		//line->AddChild(line2);
		//line2->SetColor1(color1);
		//line2->SetColor2(color2);
		};

	//AddLine(0, 0, 0, 0, 200, 0, rolling::Color::RED, rolling::Color::BLUE);
	//AddLine(0, 0, 0, 200, 0, 0, rolling::Color::RED, rolling::Color::BLUE);
	//AddLine(200, 200, 0, 0, 200, 0, rolling::Color::RED, rolling::Color::BLUE);
	//AddLine(200, 200, 0, 200, 0, 0, rolling::Color::RED, rolling::Color::BLUE);

	rolling::World3D* world3d = new rolling::World3D();
	app.GetScene().AddWorld(world3d);

	rolling::base::SharedPtr<rolling::Cube> cube = rolling::Cube::create();
	cube->SetPosition(0, 0, 0);
	cube->SetScale(1, 1, 1);
	cube->SetWidth(100);
	cube->SetHeight(100);
	cube->SetDepth(100);
	cube->SetColor(0, rolling::Color::RED);
	cube->SetColor(1, rolling::Color::GREEN);
	cube->SetColor(2, rolling::Color::BLUE);
	cube->SetColor(3, rolling::Color::WHITE);
	cube->SetColor(4, rolling::Color::YELLOW);
	cube->SetColor(5, rolling::Color::CYAN);
	cube->SetColor(6, rolling::Color::MAGENTA);
	cube->SetColor(7, rolling::Color::WHITE);
	world3d->AddNode(cube);

	//rolling::SharedPtr<rolling::Line> line = rolling::SharedPtr<rolling::Line>(new rolling::Line());

	//AddLine(0, -100, 100, 0, 100, 100);
	//AddLine(-100, 0, 100, 100, 0, 100);

	//AddLine(0, -100, 200, 0, 100, 200);
	//AddLine(-100, 0, 200, 100, 0, 200);

	//AddLine(-100, -100, 100, 100, -100, 100);
	//AddLine(-100, -100, 100, -100, 100, 100);
	//AddLine(100, 100, 100, 100, -100, 100);
	//AddLine(100, 100, 100, -100, 100, 100);

	//AddLine(-100, -100, 200, 100, -100, 200);
	//AddLine(-100, -100, 200, -100, 100, 200);
	//AddLine(100, 100, 200, 100, -100, 200);
	//AddLine(100, 100, 200, -100, 100, 200);

	//AddLine(-100, 0, 0, 100, 0, 0);
	//AddLine(0, -100, 0, 0, 100, 0);
	//AddLine(0, 0, -100, 0, 0, 100);


	//AddLine(0, 0, 0, 200, 0, 0);
	//AddLine(200, 0, 0, 200, 200, 0);
	//AddLine(200, 200, 0, 0, 200, 0);
	//AddLine(0, 200, 0, 0, 0, 0);

	//AddLine(0, 0, 200, 200, 0, 200);
	//AddLine(200, 0, 200, 200, 200, 200);
	//AddLine(200, 200, 200, 0, 200, 200);
	//AddLine(0, 200, 200, 0, 0, 200);
	
	//AddLine(0, 0, 0, 0, 0, 200);
	//AddLine(200, 0, 0, 200, 0, 200);
	//AddLine(200, 200, 0, 200, 200, 200);
	//AddLine(0, 200, 0, 0, 200, 200);
	
	//auto AddLine2D = [&](int x1, int y1, int x2, int y2, rolling::Color color = rolling::Color::WHITE) {
	//	AddLine(x1, y1, 0, x2, y2, 0, color);
	//	};

	//AddLine2D(000, 100, 200, 200, rolling::Color::GREEN);
	//AddLine2D(400, 300, 200, 200);
	//AddLine2D(400, 100, 200, 200);
	//AddLine2D(000, 300, 200, 200);

	//AddLine2D(100, 000, 200, 200);
	//AddLine2D(300, 400, 200, 200);
	//AddLine2D(100, 400, 200, 200);
	//AddLine2D(300, 000, 200, 200);

	//AddLine2D(0, 200, 400, 200);
	//AddLine2D(200, 400, 200, 0);
	//AddLine2D(0, 0, 400, 400);
	//AddLine2D(0, 400, 400, 0);

	app.MainLoop();
	return 0;
}
