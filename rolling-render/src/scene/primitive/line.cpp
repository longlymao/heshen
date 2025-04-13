#include "line.h"

#include "scene/world/world2d.h"
#include "scene/camera/camera.h"

namespace rrender {
	void Line::Render() {
		VertexShader();
		FragmentShader();
	}
	void Line::Update() {

	}
	void Line::SetPos1(const rmath::VectorF3& pos) {
		pos1 = { pos[0], pos[1], pos[2], 1};
	}
	void Line::SetPos2(const rmath::VectorF3& pos) {
		pos2 = { pos[0], pos[1], pos[2], 1 };
	}

	void Line::SetWorld(World2D* world)
	{
		m_World = world;
	}

	void Line::BresenhamX(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image) 
	{
		int x = x1;
		int y = y1;

		int dh = y2 - y1;
		int dw = dh > 0 ? x2 - x1 : x1 - x2;
		int yStep = dh > 0 ? 1 : -1;

		int ee = 0;

		image.Set(x, y, 0xFF00FF00);

		while (x < x2)
		{
			x++;
			if (abs(ee + dh * 2) < abs(dw * 2)) {
				ee += dh * 2;
			}
			else {
				y += yStep;
				ee += 2 * dh - 2 * dw;
			}

			image.Set(x, y, 0xFF00FF00);
		}
	}

	void Line::BresenhamY(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image)
	{
		int x = x1;
		int y = y1;

		int dw = x2 - x1;
		int dh = dw > 0 ? y2 - y1 : y1 - y2;
		int xStep = dw > 0 ? 1 : -1;

		int ee = 0;

		image.Set(x, y, 0xFF00FF00);

		while (y < y2)
		{
			y++;
			if (abs(ee + dw * 2) < abs(dh * 2)) {
				ee += dw * 2;
			}
			else {
				x += xStep;
				ee += 2 * dw - 2 * dh;
			}

			image.Set(x, y, 0xFF00FF00);
		}
	}

	void Line::VertexShader()
	{
		auto viewMatrix = m_World->GetCamera().GetViewMatrix();
		tempPos1 = viewMatrix * pos1;
		tempPos2 = viewMatrix * pos2;

		auto orthographicMatrix = m_World->GetCamera().GetOrthgraphicMatrix();
		tempPos1 = orthographicMatrix * tempPos1;
		tempPos2 = orthographicMatrix * tempPos2;

		//auto projectionMatrix = m_World->GetCamera().GetProjectionMatrix();

		//tempPos1 = projectionMatrix * tempPos1;
		//tempPos2 = projectionMatrix * pos2;
	}

	void Line::FragmentShader()
	{
		int x1 = 400 * (1 + tempPos1[0]);
		int y1 = 300 * (1 + tempPos1[1]);
		int x2 = 400 * (1 + tempPos2[0]);
		int y2 = 300 * (1 + tempPos2[1]);

		auto& image = m_World->GetImage();

		if (abs(y2 - y1) > abs(x2 - x1)) {
			if (y1 > y2) {
				BresenhamY(x2, y2, x1, y1, image);
			}
			else {
				BresenhamY(x1, y1, x2, y2, image);
			}
		}
		else {
			if (x1 > x2) {
				BresenhamX(x2, y2, x1, y1, image);
			}
			else {
				BresenhamX(x1, y1, x2, y2, image);
			}
		}
	}
}