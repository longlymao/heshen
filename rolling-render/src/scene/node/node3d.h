/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"

namespace rolling {
	class World3D;

	class Node3D {
	public:
		Node3D() = default;
		virtual ~Node3D() = default;
		virtual void Render(rolling::Matrix4x4& modelViewTransform) = 0;
		virtual void Update(rolling::Matrix4x4& modelViewTransform) = 0;

		rolling::Matrix4x4 GetLocalTransform();
		void SetPosition(float x, float y, float z);
		void SetScale(float x, float y, float z);

		void SetWorld(World3D* world) { m_World = world; }
		World3D* GetWorld() { return m_World; }

	protected:
		World3D* m_World = nullptr;

		rolling::Matrix4x4 m_LocalTransform;
		bool m_LocalTransformDirty = true;

		rolling::VectorF4 m_LocalPosition = { 0, 0, 0, 1 };
		rolling::VectorF4 m_LocalScale = { 1, 1, 1, 1 };
		rolling::VectorF4 m_LocalRotation = { 0, 0, 0, 1 };
	};
}