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
		Q, E,
		MouseLeft, MouseRight, MouseMiddle,
	};

	enum class KeyEvent {
		NONE,
		KEY_DOWN,
		KEY_UP,

		MOUSE_MOVE,

		FOCUS_IN,
		FOCUS_OUT,

		MOUSE_LEFT_DOWN,
		MOUSE_LEFT_UP,
		MOUSE_RIGHT_DOWN,
		MOUSE_RIGHT_UP,
		MOUSE_MIDDLE_DOWN,
		MOUSE_MIDDLE_UP,
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

		void HandleInputEvent(KeyEvent event, KeyCode key, int x, int y);

		bool IsKeyPressed(KeyCode key);
		void ClearKeyState(KeyCode key);
		void ClearKeyStates();

		int GetMouseX() const { return mouseX; }
		int GetMouseY() const { return mouseY; }
		int GetMouseMoveX() const { return mouseMoveX; }
		int GetMouseMoveY() const { return mouseMoveY; }

		void EndFrame();

	private:
		std::unordered_map<int, InputHandler> m_Handlers;
		std::unordered_map<KeyCode, bool> m_KeyStates;

		int sid = 0;

		int mouseX = 0;
		int mouseY = 0;
		int mouseMoveX = 0;
		int mouseMoveY = 0;
		bool firstMouseMove = true;
	};

}