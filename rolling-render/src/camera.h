/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "matrix.h"

namespace rrender {
	class Camera
	{
	public:
		Matrix4x4 GetViewMatrix() const;

	private:
		VectorF3 eye;
		VectorF3 target;
		VectorF3 up;
	};
};