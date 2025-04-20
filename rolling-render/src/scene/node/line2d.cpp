#include "line2d.h"
#include "scene/world/world2d.h"
#include "renderer/shader/shaderbuffer.h"

namespace rolling {
	Line2D::Line2D()
		: Node2D()
	{
	}

	Line2D::~Line2D()
	{
	}

	void Line2D::Update(rolling::Matrix4x4& modelViewTransform)
	{
		Node2D::Update(modelViewTransform);
	}

	void Line2D::Render(rolling::Matrix4x4& modelViewTransform)
	{
		auto& CommandList = m_World->GetCommandList();
		ShaderCommand cmd;
		cmd.primitiveType = PrimitiveType::LINE;
		cmd.buffer.AddData(modelViewTransform);
		cmd.buffer.AddData(pos1);
		cmd.buffer.AddData(pos2);
		cmd.buffer.AddData(color1);
		cmd.buffer.AddData(color2);
		CommandList.AddCommand(std::move(cmd));
	}

	void Line2D::SetPos1(float x, float y)
	{
		pos1[0] = x;
		pos1[1] = y;
		pos1[2] = 0;
		pos1[3] = 1;
	}

	void Line2D::SetPos2(float x, float y)
	{
		pos2[0] = x;
		pos2[1] = y;
		pos2[2] = 0;
		pos2[3] = 1;
	}

	void Line2D::SetColor(const Color& color)
	{
		color1 = color;
		color2 = color;
	}

	void Line2D::SetColor1(const Color& color)
	{
		color1 = color;
	}

	void Line2D::SetColor2(const Color& color)
	{
		color2 = color;
	}
};