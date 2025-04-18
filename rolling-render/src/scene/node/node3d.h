/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"

namespace rrender {
	class World3D;

	class Node3D {
	public:
		Node3D() = default;
		virtual ~Node3D() = default;
		virtual void Render() = 0;
		virtual void Update() = 0;

		void SetPos(const rmath::VectorF3& pos) { m_Pos = pos; }
		void SetWorld(World3D* world) { m_World = world; }
		World3D* GetWorld() { return m_World; }

	private:
		World3D* m_World = nullptr;
		rmath::VectorF3 m_Pos = { 0, 0, 0 };
	};
}