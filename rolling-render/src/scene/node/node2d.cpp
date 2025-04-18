#include "node2d.h"

namespace rrender {
	Node2D::Node2D() {

	}

	Node2D::~Node2D() {
		for (auto& child : m_Children) {
			child->SetParent(nullptr);
		}
		m_Children.clear();
	}

	void Node2D::Update(rmath::Matrix4x4& modelViewTransform)
	{
		rmath::Matrix4x4 newModelViewTransform = modelViewTransform * GetLocalTransform();
		Render(newModelViewTransform);

		for (auto& child : m_Children) {
			child->Update(newModelViewTransform);
		}
	}

	void Node2D::Render(rmath::Matrix4x4& modelViewTransform)
	{
	}

	void Node2D::AddChild(SharedPtr<Node2D> child)
	{
		m_Children.push_back(child);
		child->SetParent(this->ShareFromThis());
	}

	void Node2D::RemoveChild(SharedPtr<Node2D> child)
	{
		auto it = std::find(m_Children.begin(), m_Children.end(), child);
		if (it != m_Children.end()) {
			m_Children.erase(it);
			child->SetParent(nullptr);
		}
	}

	void Node2D::SetParent(WeakPtr<Node2D> parent)
	{
		m_Parent = parent;
		if (parent) {
			m_World = parent->m_World;
		}
		else {
			m_World = nullptr;
		}
	}

	rmath::Matrix4x4 Node2D::GetLocalTransform()
	{
		if (m_LocalTransformDirty) {
			m_LocalTransform = rmath::Matrix4x4::Identity();
			m_LocalTransformDirty = false;

			rmath::Matrix4x4 translation = rmath::Matrix4x4::Identity();
			translation.SetColumn(3, m_LocalPosition);

			rmath::Matrix4x4 rotation = rmath::Matrix4x4::Identity();

			rmath::Matrix4x4 scale = rmath::Matrix4x4::Identity();
			scale[0][0] = m_LocalScale[0];
			scale[1][1] = m_LocalScale[1];

			m_LocalTransform = translation * rotation * scale;
		}
		return m_LocalTransform;
	}
	void Node2D::SetPosition(float x, float y)
	{
		m_LocalPosition[0] = x;
		m_LocalPosition[1] = y;
		m_LocalPosition[2] = 0;
		m_LocalPosition[3] = 1;
		m_LocalTransformDirty = true;
	}
	void Node2D::SetScale(float x, float y)
	{
		m_LocalScale[0] = x;
		m_LocalScale[1] = y;
		m_LocalScale[2] = 1;
		m_LocalScale[3] = 1;
		m_LocalTransformDirty = true;
	}
};