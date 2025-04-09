#include "world2d.h"

namespace rrender {
	World2D::World2D():
		image(800, 600)
	{
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
			line->Render(image);
		}
	}
	void World2D::Update()
	{
	}
	void World2D::AddLine(Line* line)
	{
		lines.push_back(line);
	}
	void World2D::SetResolution(int width, int height)
	{
		image = rmath::Image<unsigned int>(width, height);
	}
	const rmath::Image<unsigned int>& World2D::GetImage() const
	{
		return image;
	}
};