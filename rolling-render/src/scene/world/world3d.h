/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "scene/camera/camera3d.h"
#include "image.h"

#include "scene/node/node3d.h"

#include "renderer/commandlist.h"

#include "app/input/inputmanager.h"

namespace rolling {
	class Scene;

	class InputHelper {
	public:
		InputHelper() = delete;
		InputHelper(InputHandler handle) {
			handleId = InputManager::GetInstance().RegisterHandler(handle);
		}
		~InputHelper() {
			InputManager::GetInstance().UnregisterHandler(handleId);
		}
	private:
		int handleId = 0;
	};

	class World3D {
	public:
		World3D();
		~World3D();

		void Render();
		void Update();

		void SetScene(Scene* scene);

		Camera3D& GetCamera();

		void AddNode(SharedPtr<Node3D> node);

		Rect<int> GetResolution();
		CommandList& GetCommandList();
		
	private:
		void OnInputEvent(KeyCode key, KeyEvent event);

	private:
		Scene* m_Scene = nullptr;
		Camera3D camera;
		std::vector<SharedPtr<Node3D>> m_Nodes;
		CommandList m_CommandList;

		std::unique_ptr<InputHelper> m_InputHelper = nullptr;
	};
}