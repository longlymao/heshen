#include "line3d.h"
#include "scene/world/world3d.h"
#include "renderer/shader/shaderbuffer.h"

namespace rolling {
	Line3D::Line3D()
	{
	}

	Line3D::~Line3D()
	{
	}

	void Line3D::Update(rolling::Matrix4x4& modelViewTransform)
	{
	}

	void Line3D::Render(rolling::Matrix4x4& modelViewTransform)
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

	void Line3D::SetPos1(const rolling::VectorF3& pos)
	{
		pos1[0] = pos[0];
		pos1[1] = pos[1];
		pos1[2] = pos[2];
		pos1[3] = 1;
	}

	void Line3D::SetPos2(const rolling::VectorF3& pos)
	{
		pos2[0] = pos[0];
		pos2[1] = pos[1];
		pos2[2] = pos[2];
		pos2[3] = 1;
	}

	void Line3D::SetColor(const Color& color)
	{
		color1 = color;
		color2 = color;
	}

	void Line3D::SetColor1(const Color& color)
	{
		color1 = color;
	}

	void Line3D::SetColor2(const Color& color)
	{
		color2 = color;
	}
};