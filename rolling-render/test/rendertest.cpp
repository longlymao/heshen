#include "renderapp.h"

#include "matrix.h"
#include "base/color.h"

#include <memory>
#include "base/ref.h"

#include "scene/node/line2d.h"

static int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	rolling::RenderApp app;
	app.Init();
	app.SetFrameRate(60);

	std::shared_ptr<rolling::Camera> c;

	rolling::World2D* world = new rolling::World2D();

	//auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2, rolling::Color color = rolling::Color::WHITE) {
	//	rolling::Line* line = new rolling::Line();
	//	line->SetPos1(rolling::VectorF3(x1, y1, z1));
	//	line->SetPos2(rolling::VectorF3(x2, y2, z2));
	//	line->SetColor(color);
	//	world->AddLine(line);
	//	};

	auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2, 
		rolling::Color color1 = rolling::Color::WHITE, rolling::Color color2 = rolling::Color::WHITE) {
		rolling::SharedPtr<rolling::Line2D> line = rolling::Line2D::create();
		line->SetPosition(70, 130);
		line->SetPos1(x1, y1);
		line->SetPos2(x2, y2);
		world->AddNode(line);
		line->SetColor1(color1);
		line->SetColor2(color2);

		rolling::SharedPtr<rolling::Line2D> line2 = rolling::Line2D::create();
		line2->SetPosition(10, 10);
		line2->SetScale(2, 2);
		line2->SetPos1(x1, y1);
		line2->SetPos2(x2, y2);
		line->AddChild(line2);
		line2->SetColor1(color1);
		line2->SetColor2(color2);
		};

	AddLine(0, 0, 0, 0, 200, 0, rolling::Color::RED, rolling::Color::BLUE);
	AddLine(0, 0, 0, 200, 0, 0, rolling::Color::RED, rolling::Color::BLUE);
	AddLine(200, 200, 0, 0, 200, 0, rolling::Color::RED, rolling::Color::BLUE);
	AddLine(200, 200, 0, 200, 0, 0, rolling::Color::RED, rolling::Color::BLUE);

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

	app.GetScene().AddWorld(world);

	app.MainLoop();
	return 0;
}
