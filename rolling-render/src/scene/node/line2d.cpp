#include "line2d.h"
#include "scene/world/world2d.h"

namespace rrender {
	Line2D::Line2D()
		: Node2D()
	{
	}

	Line2D::~Line2D()
	{
	}

	void Line2D::Update(rmath::Matrix4x4& modelViewTransform)
	{
		Node2D::Update(modelViewTransform);
	}

	void Line2D::Render(rmath::Matrix4x4& modelViewTransform)
	{
		VertexShader(modelViewTransform);
		FragmentShader();
	}

	void Line2D::SetPos1(float x, float y)
	{
		pos1[0] = x;
		pos1[1] = y;
		pos1[2] = 0;
		pos1[3] = 1;
	}

	void Line2D::SetPos2(float x, float y)
	{
		pos2[0] = x;
		pos2[1] = y;
		pos2[2] = 0;
		pos2[3] = 1;
	}

	void Line2D::VertexShader(rmath::Matrix4x4& modelViewTransform)
	{
		tempPos1 = modelViewTransform * pos1;
		tempPos2 = modelViewTransform * pos2;
	}

	void Line2D::FragmentShader()
	{
		int x1 = tempPos1[0];
		int y1 = tempPos1[1];
		int x2 = tempPos2[0];
		int y2 = tempPos2[1];

		if (abs(y2 - y1) > abs(x2 - x1)) {
			if (y1 > y2) {
				BresenhamY(x2, y2, x1, y1);
			}
			else {
				BresenhamY(x1, y1, x2, y2);
			}
		}
		else {
			if (x1 > x2) {
				BresenhamX(x2, y2, x1, y1);
			}
			else {
				BresenhamX(x1, y1, x2, y2);
			}
		}
	}

	void Line2D::BresenhamX(int x1, int y1, int x2, int y2)
	{
		auto& image = m_World->GetImage();

		uint32_t uColor = color.ToARGB();

		int x = x1;
		int y = y1;

		int dh = y2 - y1;
		int dw = dh > 0 ? x2 - x1 : x1 - x2;
		int yStep = dh > 0 ? 1 : -1;

		int ee = 0;

		image.Set(x, y, uColor);

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

			image.Set(x, y, uColor);
		}
	}
	void Line2D::BresenhamY(int x1, int y1, int x2, int y2)
	{
		auto& image = m_World->GetImage();

		uint32_t uColor = color.ToARGB();

		int x = x1;
		int y = y1;

		int dw = x2 - x1;
		int dh = dw > 0 ? y2 - y1 : y1 - y2;
		int xStep = dw > 0 ? 1 : -1;

		int ee = 0;

		image.Set(x, y, uColor);

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

			image.Set(x, y, uColor);
		}
	}
};