#include "scene.h"

namespace rolling {
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
		for (auto* world : worlds2d) {
			delete world;
		}
		worlds2d.clear();
		for (auto* world : worlds3d) {
			delete world;
		}
		worlds3d.clear();
	}

	void Scene::AddWorld(World2D* world)
	{
		worlds2d.push_back(world);
		world->SetScene(this);
	}

	void Scene::AddWorld(World3D* world)
	{
		worlds3d.push_back(world);
		world->SetScene(this);
	}

	void Scene::Update()
	{
		for (auto* world : worlds2d) {
			world->Update();
		}
		for (auto* world : worlds3d) {
			world->Update();
		}
	}

	void Scene::Render()
	{
		for (auto* world : worlds2d) {
			world->Render();
		}
		for (auto* world : worlds3d) {
			world->Render();
		}
	}
	void Scene::SetResolution(int width, int height)
	{
		resolution = {0, 0, width, height };
	}
	rolling::Rect<int> Scene::GetResolution()
	{
		return resolution;
	}
}
