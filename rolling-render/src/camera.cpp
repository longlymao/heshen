/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "camera.h"

namespace rrender {
	Matrix4x4 Camera::GetViewMatrix() const
	{
		VectorF3 _forward = (target - eye).Normalize();
		VectorF3 _right = up.Cross(_forward).Normalize();
		VectorF3 _up = _forward.Cross(_right).Normalize();

		Matrix4x4 viewMatrix;
		viewMatrix.SetRow(0, _right);
		viewMatrix.SetRow(1, _up);
		viewMatrix.SetRow(2, _forward);
		viewMatrix.SetRow(3, VectorF4(0, 0, 0, 1));
		viewMatrix[3][0] = -_right.Dot(eye);
		viewMatrix[3][1] = -_up.Dot(eye);
		viewMatrix[3][2] = -_forward.Dot(eye);

		return viewMatrix;
	}
}