#include "node3d.h"

namespace rolling {
	rolling::Matrix4x4 Node3D::GetLocalTransform()
	{
		if (m_LocalTransformDirty) {
			m_LocalTransform = rolling::Matrix4x4::Identity();
			m_LocalTransformDirty = false;

			rolling::Matrix4x4 translation = rolling::Matrix4x4::Identity();
			translation.SetColumn(3, m_LocalPosition);

			rolling::Matrix4x4 rotation = rolling::Matrix4x4::Identity();

			rolling::Matrix4x4 scale = rolling::Matrix4x4::Identity();
			scale[0][0] = m_LocalScale[0];
			scale[1][1] = m_LocalScale[1];

			m_LocalTransform = translation * rotation * scale;
		}
		return m_LocalTransform;
	}

	void Node3D::SetPosition(float x, float y, float z)
	{
		m_LocalPosition[0] = x;
		m_LocalPosition[1] = y;
		m_LocalPosition[2] = z;
		m_LocalTransformDirty = true;
	}

	void Node3D::SetScale(float x, float y, float z)
	{
		m_LocalScale[0] = x;
		m_LocalScale[1] = y;
		m_LocalScale[2] = z;
		m_LocalTransformDirty = true;
	}

}