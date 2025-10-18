#pragma once

#include <DirectXMath.h>
#include <cmath>

namespace DX12Math
{
	using namespace DirectX;

	// Constants
	constexpr float PI = 3.14159265358979323846f;
	constexpr float PI_2 = 1.57079632679489661923f;
	constexpr float PI_4 = 0.785398163397448309616f;
	constexpr float TWO_PI = 6.28318530717958647692f;
	constexpr float DEG_TO_RAD = PI / 180.0f;
	constexpr float RAD_TO_DEG = 180.0f / PI;

	// Matrix functions
	inline XMFLOAT4X4 Identity4x4()
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixIdentity());
		return result;
	}

	inline XMFLOAT4X4 Translation(float x, float y, float z)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixTranslation(x, y, z));
		return result;
	}

	inline XMFLOAT4X4 Translation(const XMFLOAT3& position)
	{
		return Translation(position.x, position.y, position.z);
	}

	inline XMFLOAT4X4 RotationX(float angle)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixRotationX(angle));
		return result;
	}

	inline XMFLOAT4X4 RotationY(float angle)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixRotationY(angle));
		return result;
	}

	inline XMFLOAT4X4 RotationZ(float angle)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixRotationZ(angle));
		return result;
	}

	inline XMFLOAT4X4 RotationRollPitchYaw(float pitch, float yaw, float roll)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
		return result;
	}

	inline XMFLOAT4X4 RotationRollPitchYaw(const XMFLOAT3& angles)
	{
		return RotationRollPitchYaw(angles.x, angles.y, angles.z);
	}

	inline XMFLOAT4X4 Scaling(float scaleX, float scaleY, float scaleZ)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixScaling(scaleX, scaleY, scaleZ));
		return result;
	}

	inline XMFLOAT4X4 Scaling(float uniformScale)
	{
		return Scaling(uniformScale, uniformScale, uniformScale);
	}

	inline XMFLOAT4X4 Scaling(const XMFLOAT3& scale)
	{
		return Scaling(scale.x, scale.y, scale.z);
	}

	inline XMFLOAT4X4 LookAt(const XMFLOAT3& eye, const XMFLOAT3& target, const XMFLOAT3& up)
	{
		XMFLOAT4X4 result;
		XMVECTOR eyeVec = XMLoadFloat3(&eye);
		XMVECTOR targetVec = XMLoadFloat3(&target);
		XMVECTOR upVec = XMLoadFloat3(&up);
		XMStoreFloat4x4(&result, XMMatrixLookAtRH(eyeVec, targetVec, upVec));
		return result;
	}

	inline XMFLOAT4X4 PerspectiveFOV(float fov, float aspectRatio, float nearZ, float farZ)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixPerspectiveFovRH(fov, aspectRatio, nearZ, farZ));
		return result;
	}

	inline XMFLOAT4X4 Orthographic(float width, float height, float nearZ, float farZ)
	{
		XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixOrthographicRH(width, height, nearZ, farZ));
		return result;
	}

	inline XMFLOAT4X4 Multiply(const XMFLOAT4X4& a, const XMFLOAT4X4& b)
	{
		XMFLOAT4X4 result;
		XMMATRIX matA = XMLoadFloat4x4(&a);
		XMMATRIX matB = XMLoadFloat4x4(&b);
		XMStoreFloat4x4(&result, XMMatrixMultiply(matA, matB));
		return result;
	}

	inline XMFLOAT4X4 Transpose(const XMFLOAT4X4& matrix)
	{
		XMFLOAT4X4 result;
		XMMATRIX mat = XMLoadFloat4x4(&matrix);
		XMStoreFloat4x4(&result, XMMatrixTranspose(mat));
		return result;
	}

	inline XMFLOAT4X4 Inverse(const XMFLOAT4X4& matrix)
	{
		XMFLOAT4X4 result;
		XMMATRIX mat = XMLoadFloat4x4(&matrix);
		XMVECTOR det = XMMatrixDeterminant(mat);
		XMStoreFloat4x4(&result, XMMatrixInverse(&det, mat));
		return result;
	}

	// Vector functions
	inline XMFLOAT3 Add(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMLoadFloat3(&a) + XMLoadFloat3(&b));
		return result;
	}

	inline XMFLOAT3 Subtract(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMLoadFloat3(&a) - XMLoadFloat3(&b));
		return result;
	}

	inline XMFLOAT3 Multiply(const XMFLOAT3& vec, float scalar)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMLoadFloat3(&vec) * scalar);
		return result;
	}

	inline XMFLOAT3 Cross(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVector3Cross(XMLoadFloat3(&a), XMLoadFloat3(&b)));
		return result;
	}

	inline float Dot(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMVECTOR dot = XMVector3Dot(XMLoadFloat3(&a), XMLoadFloat3(&b));
		return XMVectorGetX(dot);
	}

	inline float Length(const XMFLOAT3& vec)
	{
		XMVECTOR length = XMVector3Length(XMLoadFloat3(&vec));
		return XMVectorGetX(length);
	}

	inline XMFLOAT3 Normalize(const XMFLOAT3& vec)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVector3Normalize(XMLoadFloat3(&vec)));
		return result;
	}

	inline XMFLOAT3 Transform(const XMFLOAT3& vec, const XMFLOAT4X4& matrix)
	{
		XMFLOAT3 result;
		XMMATRIX mat = XMLoadFloat4x4(&matrix);
		XMStoreFloat3(&result, XMVector3Transform(XMLoadFloat3(&vec), mat));
		return result;
	}

	// Utility functions
	inline float ToRadians(float degrees)
	{
		return degrees * DEG_TO_RAD;
	}

	inline float ToDegrees(float radians)
	{
		return radians * RAD_TO_DEG;
	}

	inline float Lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	inline XMFLOAT3 Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t)
	{
		XMFLOAT3 result;
		XMStoreFloat3(&result, XMVectorLerp(XMLoadFloat3(&a), XMLoadFloat3(&b), t));
		return result;
	}

	// Quaternion functions
	inline XMFLOAT4 QuaternionIdentity()
	{
		XMFLOAT4 result;
		XMStoreFloat4(&result, XMQuaternionIdentity());
		return result;
	}

	inline XMFLOAT4 QuaternionRotationRollPitchYaw(float pitch, float yaw, float roll)
	{
		XMFLOAT4 result;
		XMStoreFloat4(&result, XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
		return result;
	}

	inline XMFLOAT4X4 QuaternionToMatrix(const XMFLOAT4& quaternion)
	{
		XMFLOAT4X4 result;
		XMVECTOR quat = XMLoadFloat4(&quaternion);
		XMStoreFloat4x4(&result, XMMatrixRotationQuaternion(quat));
		return result;
	}
}