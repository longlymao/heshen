#include "playercontrol.h"
#include "matrix.h"

#include "world3d.h"
#include "scene/camera/camera3d.h"

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

		if (!localMove.IsZero()) {
			m_World->GetCamera().MoveLocal(localMove);
		}

		// Handle mouse movement
		int mouseMoveX = InputManager::GetInstance().GetMouseMoveX();
		int mouseMoveY = InputManager::GetInstance().GetMouseMoveY();

		constexpr float sensitivity = 0.1f;

		if (mouseMoveX != 0 || mouseMoveY != 0) {
			float yaw = mouseMoveX * sensitivity;
			float pitch = mouseMoveY * sensitivity;
			m_World->GetCamera().RotateLocal(yaw, -pitch);
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