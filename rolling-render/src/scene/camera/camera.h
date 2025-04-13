/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"
#include <numbers>

namespace rrender {
	class World2D;

	class Camera
	{
	public:
		Camera();
		~Camera();

		rmath::Matrix4x4 GetViewMatrix() const;
		rmath::Matrix4x4 GetProjectionMatrix() const;
		rmath::Matrix4x4 GetViewProjectionMatrix() const;
	
		rmath::Matrix4x4 GetOrthgraphicMatrix() const;

		void SetEye(const rmath::VectorF3& e);
		void SetTarget(const rmath::VectorF3& t);
		void SetUp(const rmath::VectorF3& u);
		void SetFov(float f);
		void SetAspect(float s);
		void SetNear(float n);
		void SetFar(float f);

		void SetWorld(World2D* world);

		rmath::VectorF3 eye;
		rmath::VectorF3 target;
		rmath::VectorF3 up;

		float fov = std::numbers::pi / 3;
		float aspect = 4.0 / 3.0;
		float near = 0.1;
		float far = 100;

		World2D* m_world;
	};
};