#include "playercontrol.h"
#include "app/input/inputmanager.h"
#include "matrix.h"

#include "world3d.h"
#include "scene/camera/camera3d.h"
#include <iostream>

namespace rolling {
	PlayerControl::PlayerControl(World3D* world):
		m_World(world)
	{
	}
	PlayerControl::~PlayerControl()
	{
	}
	void PlayerControl::Init()
	{
		m_InputHelper = std::make_unique<InputHelper>(
			[this](KeyCode code, KeyEvent event) {
				OnInputEvent(code, event);
			}
		);
	}

	void PlayerControl::Update()
	{
		constexpr float speed = 10;
		VectorF3 localMove(0, 0, 0);
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::W)) {
			localMove[2] += speed;
		}
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::S)) {
			localMove[2] += -speed;
		}
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::A)) {
			localMove[0] += -speed;
		}
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::D)) {
			localMove[0] += speed;
		}
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::Q)) {
			localMove[1] += speed;
		}
		if (InputManager::GetInstance().IsKeyPressed(KeyCode::E)) {
			localMove[1] += -speed;
		}

		if (!localMove.IsZero()) {
			m_World->GetCamera().MoveLocal(localMove);
			// 实时打印摄像机位置
			auto eye = m_World->GetCamera().eye;
			auto target = m_World->GetCamera().target;
			std::cout << "Camera Eye: (" << eye[0] << ", " << eye[1] << ", " << eye[2] << ")"
					<< " Target: (" << target[0] << ", " << target[1] << ", " << target[2] << ")" << std::endl;
		}

		// Handle mouse movement
		int mouseMoveX = InputManager::GetInstance().GetMouseMoveX();
		int mouseMoveY = InputManager::GetInstance().GetMouseMoveY();

		constexpr float sensitivity = 0.1f;

		if (InputManager::GetInstance().IsKeyPressed(KeyCode::MouseRight) && (mouseMoveX != 0 || mouseMoveY != 0)) {
			float yaw = mouseMoveX * sensitivity;
			float pitch = mouseMoveY * sensitivity;
			m_World->GetCamera().RotateLocal(yaw, -pitch);

			// 实时打印摄像机位置
			auto eye = m_World->GetCamera().eye;
			auto target = m_World->GetCamera().target;
			std::cout << "Camera Eye: (" << eye[0] << ", " << eye[1] << ", " << eye[2] << ")"
					<< " Target: (" << target[0] << ", " << target[1] << ", " << target[2] << ")" << std::endl;
		}
	}

	void PlayerControl::OnInputEvent(KeyCode key, KeyEvent event)
	{
		if (event == KeyEvent::KEY_DOWN) {
			switch (key) {
			case KeyCode::W:
				// Handle W key down
				break;
			case KeyCode::A:
				// Handle A key down
				break;
			case KeyCode::S:
				// Handle S key down
				break;
			case KeyCode::D:
				// Handle D key down
				break;
			default:
				break;
			}
		}
	}
	// Handle other key events if needed
}