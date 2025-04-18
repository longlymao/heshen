/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include <cstdint>
#include <vector>
#include "base/ref.h"
#include "matrix.h"

namespace rrender {

	class World2D;

	class Node2D : public EnableShareFromThis<Node2D> {
	public:
		using thisType = Node2D;
		template <typename... Args>
		static SharedPtr<thisType> create(Args&&... args) {
			return SharedPtr<thisType>(new thisType(std::forward<Args>(args)...));
		}

	public:
		Node2D();
		virtual ~Node2D();

		Node2D(const Node2D&) = delete;
		Node2D(Node2D&&) = delete;
		Node2D& operator=(const Node2D&) = delete;
		Node2D& operator=(Node2D&&) = delete;

		virtual void Update(rmath::Matrix4x4& modelViewTransform);
		virtual void Render(rmath::Matrix4x4& modelViewTransform);

		void AddChild(SharedPtr<Node2D> child);
		void RemoveChild(SharedPtr<Node2D> child);
		void SetParent(WeakPtr<Node2D> parent);

		rmath::Matrix4x4 GetLocalTransform();
		void SetPosition(float x, float y);
		void SetScale(float x, float y);

		void SetWorld(World2D* world) {
			m_World = world;
		}

	protected:
		std::vector<rrender::SharedPtr<Node2D>> m_Children;
		rrender::WeakPtr<Node2D> m_Parent;
		World2D* m_World = nullptr;

		rmath::Matrix4x4 m_LocalTransform;
		bool m_LocalTransformDirty = true;

		rmath::VectorF4 m_LocalPosition = { 0, 0, 0, 1 };
		rmath::VectorF4 m_LocalScale = { 1, 1, 1, 1 };
		rmath::VectorF4 m_LocalRotation = { 0, 0, 0, 1 };
	};
};