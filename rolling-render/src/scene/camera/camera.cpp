/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#include "camera.h"

namespace rrender {
	Camera::Camera():
		m_world(nullptr)
	{
		eye = rmath::VectorF3(0, 0, 0);
		target = rmath::VectorF3(0, 0, -1);
		up = rmath::VectorF3(0, 1, 0);
		fov = std::numbers::pi / 3;
		aspect = 4.0f / 3.0f;
		near = 0.1f;
		far = 10000.0f;
	}
	Camera::~Camera()
	{
	}
	rmath::Matrix4x4 Camera::GetViewMatrix() const
	{
		rmath::VectorF3 _forward = (target - eye).Normalize();
		rmath::VectorF3 _right = up.Cross(_forward).Normalize();
		rmath::VectorF3 _up = _forward.Cross(_right).Normalize();

		rmath::Matrix4x4 viewMatrix;
		viewMatrix.SetRow(0, _right);
		viewMatrix.SetRow(1, _up);
		viewMatrix.SetRow(2, -_forward);
		viewMatrix.SetRow(3, rmath::VectorF4(0, 0, 0, 1));
		viewMatrix[0][3] = -_right.Dot(eye);
		viewMatrix[1][3] = -_up.Dot(eye);
		viewMatrix[2][3] = _forward.Dot(eye);

		return viewMatrix;
	}
	rmath::Matrix4x4 Camera::GetProjectionMatrix() const
	{
		rmath::Matrix4x4 projMatrix;
		//float f = 1.0f / std::tan(fov / 2.0f);
		//projMatrix[0][0] = f / aspect;
		//projMatrix[1][1] = f;
		//projMatrix[2][2] = (far + near) / (near - far);
		//projMatrix[2][3] = (2.0f * far * near) / (near - far);
		//projMatrix[3][2] = -1.0f;

		projMatrix[0][0] = near;
		projMatrix[1][1] = near;
		projMatrix[2][2] = (far + near);
		projMatrix[2][3] = -(far * near);
		projMatrix[3][2] = 1.0f;

		return projMatrix;
	}
	rmath::Matrix4x4 Camera::GetViewProjectionMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}
	rmath::Matrix4x4 Camera::GetOrthgraphicMatrix() const
	{
		rmath::Matrix4x4 orthographicMatrix;

		float left = - 400;
		float right = 400;
		float bottom = -300;
		float top = 300;
		float nearZ = 0.0f;
		float farZ = 10000.0f;

		orthographicMatrix[0][0] = 2.0f / (right - left);
		orthographicMatrix[1][1] = 2.0f / (top - bottom);
		orthographicMatrix[2][2] = -2.0f / (farZ - nearZ);
		orthographicMatrix[0][3] = -(right + left) / (right - left);
		orthographicMatrix[1][3] = -(top + bottom) / (top - bottom);
		orthographicMatrix[2][3] = -(farZ + nearZ) / (farZ - nearZ);
		orthographicMatrix[3][3] = 1.0f;

		return orthographicMatrix;
	}
	rmath::Matrix4x4 Camera::GetOrthgraphicMatrix2() const
	{
		rmath::Matrix4x4 orthographicMatrix;
		float top = near * tan(fov / 2.0f);
		float bottom = -top;
		float left = bottom * aspect;
		float right = top * aspect;
		float nearZ = near;
		float farZ = far;

		orthographicMatrix[0][0] = 2.0f / (right - left);
		orthographicMatrix[1][1] = 2.0f / (top - bottom);
		orthographicMatrix[2][2] = -2.0f / (farZ - nearZ);
		orthographicMatrix[0][3] = -(right + left) / (right - left);
		orthographicMatrix[1][3] = -(top + bottom) / (top - bottom);
		orthographicMatrix[2][3] = -(farZ + nearZ) / (farZ - nearZ);
		orthographicMatrix[3][3] = 1.0f;

		return orthographicMatrix;
	}
	void Camera::SetEye(const rmath::VectorF3& e)
	{
		eye = e;
	}
	void Camera::SetTarget(const rmath::VectorF3& t)
	{
		target = t;
	}
	void Camera::SetUp(const rmath::VectorF3& u)
	{
		up = u;
	}
	void Camera::SetFov(float f)
	{
		fov = f;
	}
	void Camera::SetAspect(float s)
	{
		aspect = s;
	}
	void Camera::SetNear(float n)
	{
		near = n;
	}
	void Camera::SetFar(float f)
	{
		far = f;
	}
	void Camera::SetWorld(World2D* world)
	{
		m_world = world;
	}
}