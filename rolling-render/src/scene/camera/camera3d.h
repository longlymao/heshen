/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"
#include <numbers>

namespace rolling {
	class World3D;

	class Camera3D {

	public:
		Camera3D();
		~Camera3D();

		rolling::Matrix4x4 GetViewMatrix() const;
		rolling::Matrix4x4 GetProjectionMatrix() const;
		rolling::Matrix4x4 GetViewProjectionMatrix() const;
		rolling::Matrix4x4 GetOrthgraphicMatrix() const;

		void SetEye(const rolling::VectorF3& e);
		void SetTarget(const rolling::VectorF3& t);
		void SetUp(const rolling::VectorF3& u);
		void SetFov(float f);
		void SetAspect(float s);
		void SetNear(float n);
		void SetFar(float f);

		void SetWorld(World3D* world);

		void MoveLocal(const rolling::VectorF3& localMove);
		void RotateLocal(float yaw, float pitch);

		rolling::VectorF3 eye;
		rolling::VectorF3 target;
		rolling::VectorF3 up;

		float fov = std::numbers::pi / 3;
		float aspect = 4.0 / 3.0;
		float near = 0.1;
		float far = 100;

		World3D* m_world;
	};
}