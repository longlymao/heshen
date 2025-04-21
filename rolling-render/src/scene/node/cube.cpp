#include "cube.h"
#include "scene/world/world3d.h"
#include "renderer/shader/shaderbuffer.h"

namespace rolling {
	Cube::Cube()
	{
	}

	Cube::~Cube()
	{
	}

	void Cube::Update(rolling::Matrix4x4& modelViewTransform)
	{
	}

	void Cube::Render(rolling::Matrix4x4& modelViewTransform)
	{
		auto& CommandList = m_World->GetCommandList();

		VectorF4 vertex[vertexCount] = {
			{ 0, 0, 0, 1 },
			{ 0, height, 0, 1 },
			{ 0, height, depth, 1 },
			{ 0, 0, depth, 1 },
			{ width, 0, 0, 1 },
			{ width, height, 0, 1 },
			{ width, height, depth, 1 },
			{ width, 0, depth, 1 },
		};

		std::pair<int, int> edges[edgeCount] = {
			{0, 1}, {1, 2}, {2, 3}, {3, 0}, 
			{4, 5}, {5, 6}, {6, 7}, {7, 4},
			{0, 4}, {1, 5}, {2, 6}, {3, 7},
		};

		ShaderCommand cmd;
		cmd.primitiveType = PrimitiveType::LINE;
		cmd.buffer.AddData(modelViewTransform);

		for (auto [i, j] : edges) {
			cmd.buffer.AddData(vertex[i]);
			cmd.buffer.AddData(vertex[j]);
			cmd.buffer.AddData(colors[i]);
			cmd.buffer.AddData(colors[j]);
		}

		CommandList.AddCommand(std::move(cmd));
	}

	void Cube::SetColor(int idx, Color color)
	{
		if (idx < 0 || idx >= vertexCount) {
			return;
		}
		colors[idx] = color;
	}

	void Cube::SetWidth(int width)
	{
		this->width = width;
	}

	void Cube::SetHeight(int height)
	{
		this->height = height;
	}

	void Cube::SetDepth(int depth)
	{
		this->depth = depth;
	}
};