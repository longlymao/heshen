#include "world2d.h"

namespace rrender {
	World2D::World2D():
		image(800, 600)
	{
		camera.SetWorld(this);
		rootNode = Node2D::create();
		rootNode->SetWorld(this);
	}
	World2D::~World2D()
	{
		for (auto* line : lines) {
			delete line;
		}
		lines.clear();
	}
	void World2D::Render()
	{
		for (auto* line : lines) {
			line->Render();
		}

		rmath::Matrix4x4 modelViewTransform = camera.GetViewMatrix();
		rootNode->Update(modelViewTransform);
	}
	void World2D::Update()
	{
	}
	void World2D::AddLine(Line* line)
	{
		lines.push_back(line);
		line->SetWorld(this);
	}
	void World2D::SetResolution(int width, int height)
	{
		image = rmath::Image<unsigned int>(width, height);
		camera.SetAspect(static_cast<float>(width) / static_cast<float>(height));
		camera.SetFov(std::numbers::pi / 2);
		camera.SetNear(1.0f);
		camera.SetFar(1000.0f);
		camera.SetEye(rmath::VectorF3(0, 0, -100));
		camera.SetTarget(rmath::VectorF3(camera.eye[0], camera.eye[1], 0));
		camera.SetUp(rmath::VectorF3(0, 1, 0)); 
	}
	const rmath::Image<unsigned int>& World2D::GetImage() const
	{
		return image;
	}
	Camera& World2D::GetCamera()
	{
		return camera;
	}
	rmath::Image<unsigned int>& World2D::GetImage()
	{
		return image;
	}
	void World2D::AddNode(SharedPtr<Node2D> node)
	{
		rootNode->AddChild(node);
	}
};