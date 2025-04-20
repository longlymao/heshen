/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"
#include "base/color.h"

namespace rolling
{
	class World2D;

	class Line {
	public:
		void Render();
		void Update();
		void SetPos1(const rolling::VectorF3& pos);
		void SetPos2(const rolling::VectorF3& pos);

		void SetWorld(World2D* world);

		void SetColor(const Color& color) {
			this->color = color;
		}

	private:
		rolling::VectorF4 pos1;
		rolling::VectorF4 pos2;

		Color color = Color(0xFFFFFFFF);

		World2D* m_World = nullptr;

	private:
		void BresenhamX(int x1, int y1, int x2, int y2, rolling::Image<unsigned int>& image);
		void BresenhamY(int x1, int y1, int x2, int y2, rolling::Image<unsigned int>& image);

		void VertexShader();
		void FragmentShader();

		rolling::VectorF4 tempPos1;
		rolling::VectorF4 tempPos2;
	};
}