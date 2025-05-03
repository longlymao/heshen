#include "inputmanager.h"
#include <Windows.h>

namespace rolling {
	InputManager& InputManager::GetInstance() {
		static InputManager instance;
		return instance;
	}
	KeyCode InputManager::TransWin32KeyToKeyCode(int key)
	{
		switch (key) {
		case 0x57: return KeyCode::W;
		case 0x41: return KeyCode::A;
		case 0x53: return KeyCode::S;
		case 0x44: return KeyCode::D;
		case 0x51: return KeyCode::Q;
		case 0x45: return KeyCode::E;
		default: return KeyCode::NONE; // Default to W if not recognized
		}
	}
	KeyEvent InputManager::TransWin32KeyEventToKeyEvent(int event)
	{
		switch (event) {
		case WM_KEYDOWN: return KeyEvent::KEY_DOWN;
		case WM_KEYUP: return KeyEvent::KEY_UP;
		case WM_SETFOCUS: return KeyEvent::FOCUS_IN;
		case WM_KILLFOCUS: return KeyEvent::FOCUS_OUT;
		case WM_MOUSEMOVE: return KeyEvent::MOUSE_MOVE;
		case WM_LBUTTONDOWN: return KeyEvent::MOUSE_LEFT_DOWN;
		case WM_LBUTTONUP: return KeyEvent::MOUSE_LEFT_UP;
		case WM_RBUTTONDOWN: return KeyEvent::MOUSE_RIGHT_DOWN;
		case WM_RBUTTONUP: return KeyEvent::MOUSE_RIGHT_UP;
		case WM_MBUTTONDOWN: return KeyEvent::MOUSE_MIDDLE_DOWN;
		case WM_MBUTTONUP: return KeyEvent::MOUSE_MIDDLE_UP;
		default: return KeyEvent::NONE; // Default to KEY_DOWN if not recognized
		}
	}
	int InputManager::RegisterHandler(InputHandler handler)
	{
		sid++;
		m_Handlers[sid] = handler;
		return sid;
	}
	void InputManager::UnregisterHandler(int id)
	{
		auto it = m_Handlers.find(id);
		if (it != m_Handlers.end()) {
			m_Handlers.erase(it);
		}
	}
	void InputManager::ProcessWin32Event(KeyCode key, KeyEvent event)
	{
		if (event == KeyEvent::KEY_DOWN) {
			m_KeyStates[key] = true;
		}
		else if (event == KeyEvent::KEY_UP) {
			m_KeyStates[key] = false;
		}

		for (const auto& handler : m_Handlers) {
			handler.second(key, event);
		}
	}

	void InputManager::HandleInputEvent(KeyEvent event, KeyCode key, int x, int y)
	{
		switch (event)
		{
		case rolling::KeyEvent::NONE:
			break;
		case rolling::KeyEvent::KEY_DOWN:
			break;
		case rolling::KeyEvent::KEY_UP:
			break;
		case rolling::KeyEvent::MOUSE_MOVE:
		{
			if (firstMouseMove) {
				mouseX = x;
				mouseY = y;
				firstMouseMove = false;
			}
			else {
				mouseMoveX = x - mouseX;
				mouseMoveY = y - mouseY;
				mouseX = x;
				mouseY = y;
			}
		}
			break;
		case rolling::KeyEvent::FOCUS_IN:
		{
			firstMouseMove = true;
		}
			break;
		case rolling::KeyEvent::FOCUS_OUT:
		{
			ClearKeyStates();
		}
			break;
		case rolling::KeyEvent::MOUSE_LEFT_DOWN:
		{
			m_KeyStates[KeyCode::MouseLeft] = true;
		}
			break;
		case rolling::KeyEvent::MOUSE_LEFT_UP:
		{
			m_KeyStates[KeyCode::MouseLeft] = false;
		}
			break;
		case rolling::KeyEvent::MOUSE_RIGHT_DOWN:
		{
			m_KeyStates[KeyCode::MouseRight] = true;
		}
			break;
		case rolling::KeyEvent::MOUSE_RIGHT_UP:
		{
			m_KeyStates[KeyCode::MouseRight] = false;
		}
			break;
		case rolling::KeyEvent::MOUSE_MIDDLE_DOWN:
		{
			m_KeyStates[KeyCode::MouseMiddle] = true;
		}
			break;
		case rolling::KeyEvent::MOUSE_MIDDLE_UP:
		{
			m_KeyStates[KeyCode::MouseMiddle] = false;
		}
			break;
		default:
			break;
		}
	}
	bool InputManager::IsKeyPressed(KeyCode key)
	{
		auto it = m_KeyStates.find(key);
		if (it != m_KeyStates.end()) {
			return it->second;
		}
		return false;
	}
	void InputManager::ClearKeyState(KeyCode key)
	{
		auto it = m_KeyStates.find(key);
		if (it != m_KeyStates.end()) {
			it->second = false;
		}
	}
	void InputManager::ClearKeyStates()
	{
		for (auto& it : m_KeyStates) {
			it.second = false;
		}
	}

	void InputManager::EndFrame()
	{
		mouseMoveX = 0;
		mouseMoveY = 0;
	}
}