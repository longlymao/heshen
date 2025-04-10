#include "line.h"

namespace rrender {
	void Line::Render(rmath::Image<unsigned int>& image) {
		int x1 = pos1[0];
		int y1 = pos1[1];
		int x2 = pos2[0];
		int y2 = pos2[1];
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
	void Line::Update() {

	}
	void Line::SetPos1(const rmath::VectorF3& pos) {
		pos1 = pos;
	}
	void Line::SetPos2(const rmath::VectorF3& pos) {
		pos2 = pos;
	}

	void Line::BresenhamX(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image) 
	{
		int x = x1;
		int y = y1;

		int dw = x2 - x1;
		int dh = y2 - y1;

		if (dh < 0) {
			dw = -dw;
		}

		int yy = 0;
		int ee = 0;

		while (x < x2)
		{
			x++;
			if (abs(ee + dh * 2) < abs(dw * 2)) {
				ee += dh * 2;
			}
			else {
				yy += 2 * abs(dw);
				ee += 2 * dh - 2 * dw;
			}
			y = y1 + yy / (2 * dw);

			image.Set(x, y, 0xFF00FF00);
		}
	}

	void Line::BresenhamY(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image)
	{
		int x = x1;
		int y = y1;

		int dw = x2 - x1;
		int dh = y2 - y1;

		if (dw < 0) {
			dh = -dh;
		}

		int xx = 0;
		int ee = 0;

		while (y < y2)
		{
			y++;
			if (abs(ee + dw * 2) < abs(dh * 2)) {
				ee += dw * 2;
			}
			else {
				xx += 2 * abs(dh);
				ee += 2 * dw - 2 * dh;
			}

			x = x1 + xx / (2 * dh);

			image.Set(x, y, 0xFF00FF00);
		}
	}
}