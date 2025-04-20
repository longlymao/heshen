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

#include "renderer/commandlist.h"

namespace rolling {
	class Scene;

	class World2D
	{
	public:
		World2D();
		~World2D();

		void Render();
		void Update();

		void SetScene(Scene* scene);

		Camera& GetCamera();

		void AddNode(SharedPtr<Node2D> node);

		rolling::Rect<int> GetResolution();

		CommandList& GetCommandList();

	private:
		Scene* m_Scene = nullptr;

		Camera camera;

		SharedPtr<Node2D> rootNode;

		CommandList m_CommandList;
	};
}