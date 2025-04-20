/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <vector>
#include <functional>

#include "world/world2d.h"

namespace rolling {

	class Scene {
	public:
		Scene();
		~Scene();

		void AddWorld(World2D* world);
		void Update();
		void Render();

		void SetResolution(int width, int height);
		rolling::Rect<int> GetResolution();

	private:
		std::vector<World2D*> worlds;

		rolling::Rect<int> resolution;
	};
}