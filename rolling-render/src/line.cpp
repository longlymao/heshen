/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "line.h"

namespace heshen
{
	void LineTool::DrawLine(Texture* texture, Pos start, Pos end)
	{
		int dw = end.x - start.x;
		int dh = end.y - start.y;

		int x = start.x;
		int y = start.y;

		int yy = start.y * 2 * dw;
		int ee = 0;

		while (x < end.x)
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

			texture->Set(x, y, 0xFF00FF00);
		}
	}
}