/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <memory>

#include "app/input/inputmanager.h"

namespace rolling {
	class World3D;

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

	class PlayerControl {
	public:
		PlayerControl() = delete;
		explicit PlayerControl(World3D* world);
		~PlayerControl();

		void Init();
		void Update();

	private:
		void OnInputEvent(KeyCode key, KeyEvent event);

	private:
		std::unique_ptr<InputHelper> m_InputHelper = nullptr;

		World3D* m_World = nullptr;
	};
}