#pragma once
#include <array>

namespace KEngine
{
	class Color
	{
	public:
		float r, g, b, a;

		// Constructors
		Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
		Color(float red, float green, float blue, float alpha = 1.0f)
			: r(red), g(green), b(blue), a(alpha) {
		}
		Color(int red, int green, int blue, int alpha = 255)
			: r(red / 255.0f), g(green / 255.0f), b(blue / 255.0f), a(alpha / 255.0f) {
		}

		// Common colors
		static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
		static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
		static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
		static Color White() { return Color(1.0f, 1.0f, 1.0f); }
		static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
		static Color Gray() { return Color(0.5f, 0.5f, 0.5f); }
		static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }
		static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }
		static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }

		// Operations
		Color operator+(const Color& other) const {
			return Color(r + other.r, g + other.g, b + other.b, a + other.a);
		}

		Color operator*(float scalar) const {
			return Color(r * scalar, g * scalar, b * scalar, a * scalar);
		}

		Color operator*(const Color& other) const {
			return Color(r * other.r, g * other.g, b * other.b, a * other.a);
		}

		// Utility methods
		void Clamp() {
			r = (r < 0.0f) ? 0.0f : (r > 1.0f) ? 1.0f : r;
			g = (g < 0.0f) ? 0.0f : (g > 1.0f) ? 1.0f : g;
			b = (b < 0.0f) ? 0.0f : (b > 1.0f) ? 1.0f : b;
			a = (a < 0.0f) ? 0.0f : (a > 1.0f) ? 1.0f : a;
		}

		Color Clamped() const {
			Color result = *this;
			result.Clamp();
			return result;
		}

		// Conversion to platform-specific types
		std::array<float, 4> ToFloat4() const { return { r, g, b, a }; }
		std::array<float, 3> ToFloat3() const { return { r, g, b }; }
	};
}