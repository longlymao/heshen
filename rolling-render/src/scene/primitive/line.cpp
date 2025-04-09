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
		int dw = x2 - x1;
		int dh = y2 - y1;

		int x = x1;
		int y = y1;

		int yy = y1 * 2 * dw;
		int ee = 0;

		while (x < x2)
		{
			x++;
			int nyy = yy + ee + dh * 2;
			if (nyy < yy + dw)
			{
				ee += dw * 2;
			}
			else
			{
				yy += 2 * dw;
				ee += 2 * dh - 2 * dw;
			}
			y = yy / (2 * dw);

			image.Set(x, y, 0xFF00FF00);
		}
	}

	void Line::BresenhamY(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image)
	{
		int dw = x2 - x1;
		int dh = y2 - y1;

		int x = x1;
		int y = y1;

		int xx = x1 * 2 * dh;
		int ee = 0;

		while (y < y2)
		{
			y++;
			int nxx = xx + ee + dw * 2;
			if (nxx < xx + dh)
			{
				ee += dh * 2;
			}
			else
			{
				xx += 2 * dh;
				ee += 2 * dw - 2 * dh;
			}
			x = xx / (2 * dh);

			image.Set(x, y, 0xFF00FF00);
		}
	}
}