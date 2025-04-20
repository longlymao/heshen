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
	using RenderFunc = std::function<void(const rolling::Image<unsigned int>&)>;

	class Scene {
	public:
		Scene();
		~Scene();

		void AddWorld(World2D* world);
		void Update();
		void Render();

		void SetResolution(int width, int height);
		void SetRenderFunc(RenderFunc func) { renderFunc = func; }
	private:
		std::vector<World2D*> worlds;
		RenderFunc renderFunc;

		rolling::Rect<int> resolution;
	};
}