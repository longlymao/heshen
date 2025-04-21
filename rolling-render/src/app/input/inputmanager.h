/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <functional>
#include <unordered_map>

namespace rolling {

	enum class KeyCode {
		NONE,
		W, A, S, D,
	};

	enum class KeyEvent {
		NONE,
		KEY_DOWN,
		KEY_UP,
	};

	using InputHandler = std::function<void(KeyCode, KeyEvent)>;

	class InputManager {
	public:
		static InputManager& GetInstance();

		static KeyCode TransWin32KeyToKeyCode(int key);
		static KeyEvent TransWin32KeyEventToKeyEvent(int event);

		int RegisterHandler(InputHandler handler);
		void UnregisterHandler(int id);

		void ProcessWin32Event(KeyCode key, KeyEvent event);

	private:
		std::unordered_map<int, InputHandler> m_Handlers;

		int sid = 0;
	};

}