/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "scene/camera/camera.h"
#include "scene/primitive/line.h"
#include "image.h"

namespace rrender {

	class World2D
	{
	public:
		World2D();
		~World2D();

		void Render();
		void Update();

		void AddLine(Line* line);

		void SetResolution(int width, int height);

		const rmath::Image<unsigned int>& GetImage() const;

		Camera& GetCamera();
		rmath::Image<unsigned int>& GetImage();

	private:
		std::vector<Line*> lines;
		Camera camera;
		rmath::Image<unsigned int> image;
	};
}