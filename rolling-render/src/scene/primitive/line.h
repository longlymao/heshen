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
	class World2D;

	class Line {
	public:
		void Render();
		void Update();
		void SetPos1(const rmath::VectorF3& pos);
		void SetPos2(const rmath::VectorF3& pos);

		void SetWorld(World2D* world);

	private:
		rmath::VectorF4 pos1;
		rmath::VectorF4 pos2;

		World2D* m_World = nullptr;

	private:
		void BresenhamX(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image);
		void BresenhamY(int x1, int y1, int x2, int y2, rmath::Image<unsigned int>& image);

		void VertexShader();
		void FragmentShader();

		rmath::VectorF4 tempPos1;
		rmath::VectorF4 tempPos2;
	};
}