/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "node2d.h"
#include "base/color.h"

namespace rrender {
	class Line2D : public Node2D {
	public:
		using thisType = Line2D;
		template <typename... Args>
		static SharedPtr<thisType> create(Args&&... args) {
			return SharedPtr<thisType>(new thisType(std::forward<Args>(args)...));
		}

	public:
		Line2D();
		virtual ~Line2D();

		Line2D(const Line2D&) = delete;
		Line2D(Line2D&&) = delete;
		Line2D& operator=(const Line2D&) = delete;
		Line2D& operator=(Line2D&&) = delete;

		void Update(rmath::Matrix4x4& modelViewTransform) override;
		void Render(rmath::Matrix4x4& modelViewTransform) override;

		void SetPos1(float x, float y);
		void SetPos2(float x, float y);

		void SetColor(const Color& color);
		void SetColor1(const Color& color);
		void SetColor2(const Color& color);

	private:
		rmath::VectorF4 pos1;
		rmath::VectorF4 pos2;
		Color color = Color::WHITE;
		Color color1 = Color::WHITE;
		Color color2 = Color::WHITE;

		void VertexShader(rmath::Matrix4x4& modelViewTransform);
		void FragmentShader();

		void BresenhamX(int x1, int y1, int x2, int y2, Color color1, Color color2);
		void BresenhamY(int x1, int y1, int x2, int y2, Color color1, Color color2);

		rmath::VectorF4 tempPos1;
		rmath::VectorF4 tempPos2;
	};
}