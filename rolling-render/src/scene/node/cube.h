/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "ref.h"
#include "node3d.h"
#include "base/color.h"

#include <vector>

namespace rolling {

	class Cube : public Node3D {
	public:
		using thisType = Cube;
		template <typename... Args>
		static base::SharedPtr<thisType> create(Args&&... args) {
			return base::SharedPtr<thisType>(new thisType(std::forward<Args>(args)...));
		}
	public:
		Cube();
		virtual ~Cube();

		Cube(const Cube&) = delete;
		Cube(Cube&&) = delete;
		Cube& operator=(const Cube&) = delete;
		Cube& operator=(Cube&&) = delete;

		void Update(rolling::Matrix4x4& modelViewTransform) override;
		void Render(rolling::Matrix4x4& modelViewTransform) override;

		void SetColor(int idx, Color color);

		void SetWidth(int width);
		void SetHeight(int height);
		void SetDepth(int depth);

	private:
		constexpr static int vertexCount = 8;
		constexpr static int edgeCount = 12;
		
		int width = 1;
		int height = 1;
		int depth = 1;

		std::vector<Color> colors = {
			Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE,
			Color::WHITE, Color::WHITE, Color::WHITE, Color::WHITE
		};
	};
}