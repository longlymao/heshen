#include "world2d.h"
#include "renderer/renderer.h"
#include "scene/scene.h"

namespace rolling {
	World2D::World2D()
	{
		camera.SetWorld(this);

		camera.SetFov(std::numbers::pi / 2);
		camera.SetNear(1.0f);
		camera.SetFar(1000.0f);
		camera.SetEye(rolling::VectorF3(0, 0, -100));
		camera.SetTarget(rolling::VectorF3(camera.eye[0], camera.eye[1], 0));
		camera.SetUp(rolling::VectorF3(0, 1, 0));

		rootNode = Node2D::create();
		rootNode->SetWorld(this);
	}
	World2D::~World2D()
	{
	}
	void World2D::Render()
	{
		m_CommandList.Clear();

		rolling::Matrix4x4 modelViewTransform = camera.GetOrthgraphicMatrix() * camera.GetViewMatrix();
		rootNode->Update(modelViewTransform);

		Renderer::GetInstance().AddCommandList(m_CommandList);
	}
	void World2D::Update()
	{
	}
	void World2D::SetScene(Scene* scene)
	{
		m_Scene = scene;
	}

	Camera2D& World2D::GetCamera()
	{
		return camera;
	}

	void World2D::AddNode(SharedPtr<Node2D> node)
	{
		rootNode->AddChild(node);
	}

	rolling::Rect<int> World2D::GetResolution()
	{
		if (m_Scene) {
			return m_Scene->GetResolution();
		}
		return rolling::Rect<int>();
	}
	CommandList& World2D::GetCommandList()
	{
		return m_CommandList;
	}
};