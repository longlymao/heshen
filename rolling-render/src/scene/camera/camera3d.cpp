#include "camera3d.h"
#include "scene/world/world3d.h"
#include <cmath>

namespace rolling {
	Camera3D::Camera3D()
	{
	}
	Camera3D::~Camera3D()
	{
	}
	rolling::Matrix4x4 Camera3D::GetViewMatrix() const
	{
		rolling::VectorF3 _forward = (target - eye).Normalize();
		rolling::VectorF3 _right = up.Cross(_forward).Normalize();
		rolling::VectorF3 _up = _forward.Cross(_right).Normalize();

		rolling::Matrix4x4 viewMatrix;
		viewMatrix.SetRow(0, _right);
		viewMatrix.SetRow(1, _up);
		viewMatrix.SetRow(2, -_forward);
		viewMatrix.SetRow(3, rolling::VectorF4(0, 0, 0, 1));
		viewMatrix[0][3] = -_right.Dot(eye);
		viewMatrix[1][3] = -_up.Dot(eye);
		viewMatrix[2][3] = _forward.Dot(eye);

		return viewMatrix;
	}

	rolling::Matrix4x4 Camera3D::GetProjectionMatrix() const
	{
		rolling::Matrix4x4 projMatrix;
		float f = 1.0f / std::tan(fov / 2.0f);
		projMatrix[0][0] = f / aspect;
		projMatrix[1][1] = f;
		projMatrix[2][2] = (far + near) / (far - near);
		projMatrix[2][3] = (2.0f * far * near) / (far - near);
		projMatrix[3][2] = -1.0f;

		return projMatrix;
	}

	rolling::Matrix4x4 Camera3D::GetViewProjectionMatrix() const
	{
		return GetProjectionMatrix() * GetViewMatrix();
	}
	rolling::Matrix4x4 Camera3D::GetOrthgraphicMatrix() const
	{
		rolling::Matrix4x4 orthographicMatrix;
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
	void Camera3D::SetEye(const rolling::VectorF3& e)
	{
		eye = e;
	}
	void Camera3D::SetTarget(const rolling::VectorF3& t)
	{
		target = t;
	}
	void Camera3D::SetUp(const rolling::VectorF3& u)
	{
		up = u;
	}
	void Camera3D::SetFov(float f)
	{
		fov = f;
	}
	void Camera3D::SetAspect(float s)
	{
		aspect = s;
	}
	void Camera3D::SetNear(float n)
	{
		near = n;
	}
	void Camera3D::SetFar(float f)
	{
		far = f;
	}
	void Camera3D::SetWorld(World3D* world)
	{
		m_world = world;
	}
	void Camera3D::MoveLocal(const rolling::VectorF3& localMove)
	{
		rolling::VectorF3 _forward = (target - eye).Normalize();
		rolling::VectorF3 _right = up.Cross(_forward).Normalize();
		rolling::VectorF3 _up = _forward.Cross(_right).Normalize();
		eye += _forward * localMove[2];
		eye += _right * localMove[0];
		eye += _up * localMove[1];
		target += _forward * localMove[2];
		target += _right * localMove[0];
		target += _up * localMove[1];
	}
	void Camera3D::RotateLocal(float yaw, float pitch)
	{
		//target[1] += 10;
		//return;
		int d = (target - eye).Length();

		rolling::VectorF3 _forward = (target - eye).Normalize();
		rolling::VectorF3 _right = up.Cross(_forward).Normalize();
		rolling::VectorF3 _up = _forward.Cross(_right).Normalize();
	
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		auto ToRadians = [](float degree) {
			return degree * std::numbers::pi / 180.0f;
			};

		auto f = (_forward + _right * std::sin(ToRadians(yaw))).Normalize();
		auto r = _up.Cross(f).Normalize();
		f = (f + _up * std::sin(ToRadians(pitch))).Normalize();
		auto u = f.Cross(r).Normalize();

		target = eye + f * d;
	}
}