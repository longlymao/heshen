#include "scene.h"

namespace rolling {
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
		for (auto* world : worlds) {
			delete world;
		}
		worlds.clear();
	}
	void Scene::AddWorld(World2D* world)
	{
		worlds.push_back(world);
		world->SetScene(this);
	}

	void Scene::Update()
	{
		for (auto* world : worlds) {
			world->Update();
		}
	}

	void Scene::Render()
	{
		for (auto* world : worlds) {
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
