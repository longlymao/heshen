/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "scene/camera/camera.h"
#include "image.h"

#include "scene/node/node2d.h"

namespace rolling {

	class World2D
	{
	public:
		World2D();
		~World2D();

		void Render();
		void Update();

		void SetResolution(int width, int height);

		const rolling::Image<unsigned int>& GetImage() const;

		Camera& GetCamera();
		rolling::Image<unsigned int>& GetImage();

		void AddNode(SharedPtr<Node2D> node);

	private:
		Camera camera;
		rolling::Image<unsigned int> image;

		SharedPtr<Node2D> rootNode;
	};
}