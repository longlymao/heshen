/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"
#include "image.h"

namespace rrender {
	class Camera
	{
	public:
		rmath::Matrix4x4 GetViewMatrix() const;

	private:
		rmath::VectorF3 eye;
		rmath::VectorF3 target;
		rmath::VectorF3 up;
	};
};