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
	rrender::RenderApp app;
	app.Init();
	app.SetFrameRate(60);

	std::shared_ptr<rrender::Camera> c;

	rrender::World2D* world = new rrender::World2D();

	//auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2, rrender::Color color = rrender::Color::WHITE) {
	//	rrender::Line* line = new rrender::Line();
	//	line->SetPos1(rmath::VectorF3(x1, y1, z1));
	//	line->SetPos2(rmath::VectorF3(x2, y2, z2));
	//	line->SetColor(color);
	//	world->AddLine(line);
	//	};

	auto AddLine = [&](int x1, int y1, int z1, int x2, int y2, int z2, 
		rrender::Color color1 = rrender::Color::WHITE, rrender::Color color2 = rrender::Color::WHITE) {
		rrender::SharedPtr<rrender::Line2D> line = rrender::Line2D::create();
		line->SetPosition(70, 130);
		line->SetPos1(x1, y1);
		line->SetPos2(x2, y2);
		world->AddNode(line);
		line->SetColor1(color1);
		line->SetColor2(color2);

		rrender::SharedPtr<rrender::Line2D> line2 = rrender::Line2D::create();
		line2->SetPosition(10, 10);
		line2->SetScale(2, 2);
		line2->SetPos1(x1, y1);
		line2->SetPos2(x2, y2);
		line->AddChild(line2);
		line2->SetColor1(color1);
		line2->SetColor2(color2);
		};

	AddLine(0, 0, 0, 0, 200, 0, rrender::Color::RED, rrender::Color::BLUE);
	AddLine(0, 0, 0, 200, 0, 0, rrender::Color::RED, rrender::Color::BLUE);
	AddLine(200, 200, 0, 0, 200, 0, rrender::Color::RED, rrender::Color::BLUE);
	AddLine(200, 200, 0, 200, 0, 0, rrender::Color::RED, rrender::Color::BLUE);

	//rrender::SharedPtr<rrender::Line> line = rrender::SharedPtr<rrender::Line>(new rrender::Line());

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
	
	//auto AddLine2D = [&](int x1, int y1, int x2, int y2, rrender::Color color = rrender::Color::WHITE) {
	//	AddLine(x1, y1, 0, x2, y2, 0, color);
	//	};

	//AddLine2D(000, 100, 200, 200, rrender::Color::GREEN);
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
