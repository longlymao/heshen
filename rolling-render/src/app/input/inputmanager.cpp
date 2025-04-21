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
		default: return KeyCode::NONE; // Default to W if not recognized
		}
	}
	KeyEvent InputManager::TransWin32KeyEventToKeyEvent(int event)
	{
		switch (event) {
		case WM_KEYDOWN: return KeyEvent::KEY_DOWN;
		case WM_KEYUP: return KeyEvent::KEY_UP;
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
		for (const auto& handler : m_Handlers) {
			handler.second(key, event);
		}
	}
}