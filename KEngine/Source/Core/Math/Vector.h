#pragma once
#include <cmath>

namespace KEngine
{
	class Vector3
	{
	public:
		float x, y, z;

		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		// Common vectors
		static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
		static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
		static Vector3 UnitX() { return Vector3(1.0f, 0.0f, 0.0f); }
		static Vector3 UnitY() { return Vector3(0.0f, 1.0f, 0.0f); }
		static Vector3 UnitZ() { return Vector3(0.0f, 0.0f, 1.0f); }

		// Operations
		Vector3 operator+(const Vector3& other) const {
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		Vector3 operator-(const Vector3& other) const {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		Vector3 operator*(float scalar) const {
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		Vector3 operator/(float scalar) const {
			return Vector3(x / scalar, y / scalar, z / scalar);
		}

		float Dot(const Vector3& other) const {
			return x * other.x + y * other.y + z * other.z;
		}

		Vector3 Cross(const Vector3& other) const {
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}

		float Length() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		Vector3 Normalized() const {
			float len = Length();
			if (len > 0.0f) {
				return Vector3(x / len, y / len, z / len);
			}
			return *this;
		}

		void Normalize() {
			float len = Length();
			if (len > 0.0f) {
				x /= len;
				y /= len;
				z /= len;
			}
		}
	};

	class Vector4
	{
	public:
		float x, y, z, w;

		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(const Vector3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

		// Operations similar to Vector3...
		Vector4 operator+(const Vector4& other) const {
			return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vector3 XYZ() const { return Vector3(x, y, z); }
	};
}