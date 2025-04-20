/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "node2d.h"
#include "base/color.h"

namespace rolling {
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

		void Update(rolling::Matrix4x4& modelViewTransform) override;
		void Render(rolling::Matrix4x4& modelViewTransform) override;

		void SetPos1(float x, float y);
		void SetPos2(float x, float y);

		void SetColor(const Color& color);
		void SetColor1(const Color& color);
		void SetColor2(const Color& color);

	private:
		rolling::VectorF4 pos1;
		rolling::VectorF4 pos2;

		Color color1 = Color::WHITE;
		Color color2 = Color::WHITE;
	};
}