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
		world->SetResolution(resolution.GetWidth(), resolution.GetHeight());
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
			if (renderFunc) {
				renderFunc(world->GetImage());
			}
		}
	}
	void Scene::SetResolution(int width, int height)
	{
		resolution = {0, 0, width, height };

		for (auto* world : worlds) {
			world->SetResolution(resolution.GetWidth(), resolution.GetHeight());
		}
	}
}
