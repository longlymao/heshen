/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"

namespace rrender
{
	class Line {
	public:
		void Render(rmath::Image<unsigned int>& image);
		void Update();
		void SetPos1(const rmath::VectorF3& pos);
		void SetPos2(const rmath::VectorF3& pos);
	private:
		rmath::VectorF3 pos1;
		rmath::VectorF3 pos2;

	private:
		void BresenhamX(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image);
		void BresenhamY(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image);
	};
}