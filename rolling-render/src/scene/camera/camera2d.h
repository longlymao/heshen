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
	class World2D;

	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		rolling::Matrix4x4 GetViewMatrix() const;
		rolling::Matrix4x4 GetProjectionMatrix() const;
		rolling::Matrix4x4 GetViewProjectionMatrix() const;
	
		rolling::Matrix4x4 GetOrthgraphicMatrix() const;
		rolling::Matrix4x4 GetOrthgraphicMatrix2() const;

		void SetEye(const rolling::VectorF3& e);
		void SetTarget(const rolling::VectorF3& t);
		void SetUp(const rolling::VectorF3& u);
		void SetFov(float f);
		void SetAspect(float s);
		void SetNear(float n);
		void SetFar(float f);

		void SetWorld(World2D* world);

		rolling::VectorF3 eye;
		rolling::VectorF3 target;
		rolling::VectorF3 up;

		float fov = std::numbers::pi / 3;
		float aspect = 4.0 / 3.0;
		float near = 0.1;
		float far = 100;

		World2D* m_world;
	};
};