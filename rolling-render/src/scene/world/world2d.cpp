#include "world2d.h"

namespace rolling {
	World2D::World2D():
		image(800, 600)
	{
		camera.SetWorld(this);
		rootNode = Node2D::create();
		rootNode->SetWorld(this);
	}
	World2D::~World2D()
	{
	}
	void World2D::Render()
	{
		rolling::Matrix4x4 modelViewTransform = camera.GetViewMatrix();
		rootNode->Update(modelViewTransform);
	}
	void World2D::Update()
	{
	}
	void World2D::SetResolution(int width, int height)
	{
		image = rolling::Image<unsigned int>(width, height);
		camera.SetAspect(static_cast<float>(width) / static_cast<float>(height));
		camera.SetFov(std::numbers::pi / 2);
		camera.SetNear(1.0f);
		camera.SetFar(1000.0f);
		camera.SetEye(rolling::VectorF3(0, 0, -100));
		camera.SetTarget(rolling::VectorF3(camera.eye[0], camera.eye[1], 0));
		camera.SetUp(rolling::VectorF3(0, 1, 0)); 
	}
	const rolling::Image<unsigned int>& World2D::GetImage() const
	{
		return image;
	}
	Camera& World2D::GetCamera()
	{
		return camera;
	}
	rolling::Image<unsigned int>& World2D::GetImage()
	{
		return image;
	}
	void World2D::AddNode(SharedPtr<Node2D> node)
	{
		rootNode->AddChild(node);
	}
};