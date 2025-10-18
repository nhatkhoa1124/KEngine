#pragma once
#include <array>
#include "Vector.h"
#include <DirectXMath.h>

namespace KEngine
{
	class Matrix
	{
	public:
		// Store in column-major order for DirectX compatibility: m[column][row]
		// This matches DirectX's XMFLOAT4X4 layout
		union
		{
			std::array<std::array<float, 4>, 4> m;  // m[column][row]
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
		};

		// Constructors
		Matrix() { SetIdentity(); }

		// Direct initialization constructor (column-major)
		Matrix(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
			: _11(m11), _12(m12), _13(m13), _14(m14),
			_21(m21), _22(m22), _23(m23), _24(m24),
			_31(m31), _32(m32), _33(m33), _34(m34),
			_41(m41), _42(m42), _43(m43), _44(m44) {
		}

		// Copy from DirectX matrix
		Matrix(const DirectX::XMFLOAT4X4& dxMatrix)
		{
			_11 = dxMatrix._11; _12 = dxMatrix._12; _13 = dxMatrix._13; _14 = dxMatrix._14;
			_21 = dxMatrix._21; _22 = dxMatrix._22; _23 = dxMatrix._23; _24 = dxMatrix._24;
			_31 = dxMatrix._31; _32 = dxMatrix._32; _33 = dxMatrix._33; _34 = dxMatrix._34;
			_41 = dxMatrix._41; _42 = dxMatrix._42; _43 = dxMatrix._43; _44 = dxMatrix._44;
		}

		// Accessors (column-major: [column][row])
		float& operator()(int row, int col) { return m[col][row]; }
		float operator()(int row, int col) const { return m[col][row]; }

		// Static factory methods using DirectXMath for accuracy and performance
		static Matrix Identity() {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixIdentity());
			return Matrix(dxMatrix);
		}

		static Matrix Translation(float x, float y, float z) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixTranslation(x, y, z));
			return Matrix(dxMatrix);
		}

		static Matrix Translation(const Vector3& translation) {
			return Translation(translation.x, translation.y, translation.z);
		}

		static Matrix RotationX(float angle) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixRotationX(angle));
			return Matrix(dxMatrix);
		}

		static Matrix RotationY(float angle) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixRotationY(angle));
			return Matrix(dxMatrix);
		}

		static Matrix RotationZ(float angle) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixRotationZ(angle));
			return Matrix(dxMatrix);
		}

		static Matrix RotationRollPitchYaw(float pitch, float yaw, float roll) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
			return Matrix(dxMatrix);
		}

		static Matrix RotationRollPitchYaw(const Vector3& angles) {
			return RotationRollPitchYaw(angles.x, angles.y, angles.z);
		}

		static Matrix Scaling(float x, float y, float z) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixScaling(x, y, z));
			return Matrix(dxMatrix);
		}

		static Matrix Scaling(const Vector3& scale) {
			return Scaling(scale.x, scale.y, scale.z);
		}

		static Matrix Scaling(float uniformScale) {
			return Scaling(uniformScale, uniformScale, uniformScale);
		}

		// View and projection matrices
		static Matrix LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&reinterpret_cast<const DirectX::XMFLOAT3&>(eye));
			DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&reinterpret_cast<const DirectX::XMFLOAT3&>(target));
			DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&reinterpret_cast<const DirectX::XMFLOAT3&>(up));
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixLookAtRH(eyeVec, targetVec, upVec));
			return Matrix(dxMatrix);
		}

		static Matrix PerspectiveFOV(float fov, float aspectRatio, float nearZ, float farZ) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio, nearZ, farZ));
			return Matrix(dxMatrix);
		}

		static Matrix Orthographic(float width, float height, float nearZ, float farZ) {
			DirectX::XMFLOAT4X4 dxMatrix;
			DirectX::XMStoreFloat4x4(&dxMatrix, DirectX::XMMatrixOrthographicRH(width, height, nearZ, farZ));
			return Matrix(dxMatrix);
		}

		// Matrix operations using DirectXMath
		Matrix operator*(const Matrix& other) const {
			DirectX::XMMATRIX mat1 = ToXMMatrix();
			DirectX::XMMATRIX mat2 = other.ToXMMatrix();
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixMultiply(mat1, mat2));
			return Matrix(result);
		}

		Matrix& operator*=(const Matrix& other) {
			*this = *this * other;
			return *this;
		}

		// Matrix - Vector4 multiplication
		Vector4 operator*(const Vector4& vector) const {
			DirectX::XMMATRIX mat = ToXMMatrix();
			DirectX::XMVECTOR vec = DirectX::XMLoadFloat4(&reinterpret_cast<const DirectX::XMFLOAT4&>(vector));
			DirectX::XMFLOAT4 result;
			DirectX::XMStoreFloat4(&result, DirectX::XMVector4Transform(vec, mat));
			return Vector4(result.x, result.y, result.z, result.w);
		}

		// Matrix - Vector3 multiplication (treat as position vector w=1)
		Vector3 operator*(const Vector3& vector) const {
			Vector4 result = (*this) * Vector4(vector, 1.0f);
			if (result.w != 0.0f) {
				return Vector3(result.x / result.w, result.y / result.w, result.z / result.w);
			}
			return Vector3(result.x, result.y, result.z);
		}

		// Matrix transpose
		Matrix Transpose() const {
			DirectX::XMMATRIX mat = ToXMMatrix();
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixTranspose(mat));
			return Matrix(result);
		}

		// Matrix inverse
		Matrix Inverse() const {
			DirectX::XMMATRIX mat = ToXMMatrix();
			DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(mat);
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, DirectX::XMMatrixInverse(&det, mat));
			return Matrix(result);
		}

		// Utility methods
		void SetIdentity() {
			*this = Identity();
		}

		bool IsIdentity() const {
			const Matrix identity = Identity();
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					if (std::abs((*this)(i, j) - identity(i, j)) > 1e-6f) {
						return false;
					}
				}
			}
			return true;
		}

		// Extract transformation components
		Vector3 GetTranslation() const {
			return Vector3(_41, _42, _43);
		}

		Vector3 GetScale() const {
			return Vector3(
				Vector3(_11, _12, _13).Length(),
				Vector3(_21, _22, _23).Length(),
				Vector3(_31, _32, _33).Length()
			);
		}

		// DirectX compatibility
		DirectX::XMMATRIX ToXMMatrix() const {
			auto matrix = ToXMFLOAT4X4();
			return DirectX::XMLoadFloat4x4(&matrix);
		}

		DirectX::XMFLOAT4X4 ToXMFLOAT4X4() const {
			return DirectX::XMFLOAT4X4{
				_11, _12, _13, _14,
				_21, _22, _23, _24,
				_31, _32, _33, _34,
				_41, _42, _43, _44
			};
		}

		// Conversion to float arrays
		std::array<float, 16> ToFloatArray() const {
			return { _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 };
		}

		// Already column-major, so same as ToFloatArray
		std::array<float, 16> ToFloatArrayColumnMajor() const {
			return ToFloatArray();
		}

		// Operator for easy DirectX conversion
		operator DirectX::XMFLOAT4X4() const {
			return ToXMFLOAT4X4();
		}
	};
}