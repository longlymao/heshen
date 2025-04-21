#include "world3d.h"
#include "scene/scene.h"
#include "renderer/renderer.h"

#include <functional>

namespace rolling {
	World3D::World3D()
	{
		camera.SetWorld(this);

		camera.SetAspect(4.0f / 3);
		camera.SetFov(std::numbers::pi / 4);
		camera.SetNear(1.0f);
		camera.SetFar(1000.0f);
		camera.SetEye(rolling::VectorF3(0, 0, -300));
		camera.SetTarget(rolling::VectorF3(0, 0, 0));
		camera.SetUp(rolling::VectorF3(0, 1, 0));

		m_InputHelper = std::make_unique<InputHelper>(
			[this](KeyCode code, KeyEvent event) {
				OnInputEvent(code, event);
			}
		);
	}

	World3D::~World3D()
	{
	}

	void World3D::Render()
	{
		m_CommandList.Clear();

		//rolling::Matrix4x4 modelViewTransform = camera.GetOrthgraphicMatrix() * camera.GetViewProjectionMatrix();
		rolling::Matrix4x4 modelViewTransform = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		for (auto& node : m_Nodes) {
			node->Render(modelViewTransform);
		}

		Renderer::GetInstance().AddCommandList(m_CommandList);
	}

	void World3D::Update()
	{
		rolling::Matrix4x4 modelViewTransform = camera.GetOrthgraphicMatrix() * camera.GetViewMatrix();

		for (auto& node : m_Nodes) {
			node->Update(modelViewTransform);
		}
	}

	void World3D::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	Camera3D& World3D::GetCamera()
	{
		return camera;
	}

	void World3D::AddNode(SharedPtr<Node3D> node)
	{
		m_Nodes.push_back(node);
		node->SetWorld(this);
	}

	Rect<int> World3D::GetResolution()
	{
		return m_Scene->GetResolution();
	}

	CommandList& World3D::GetCommandList()
	{
		return m_CommandList;
	}
	void World3D::OnInputEvent(KeyCode key, KeyEvent event)
	{
		if (event == KeyEvent::KEY_UP) {

			auto eye = camera.eye;

			switch (key)
			{
			case rolling::KeyCode::W:
				eye[1] += 10;
				break;
			case rolling::KeyCode::A:
				eye[0] -= 10;
				break;
			case rolling::KeyCode::S:
				eye[1] -= 10;
				break;
			case rolling::KeyCode::D:
				eye[0] += 10;
				break;
			default:
				break;
			}

			camera.SetEye(eye);
		}
	}
}