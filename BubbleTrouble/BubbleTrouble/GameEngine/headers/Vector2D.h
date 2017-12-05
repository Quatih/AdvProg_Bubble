#pragma once


/// 2D vector class with x and y variables of type T
/// Includes overloaded operators for convenience
template<typename T> class Vector2D {
public:
	T x;
	T y;

	Vector2D() {
		x = 0;
		y = 0;
	}

	Vector2D(const T x, const T y) {
		this->x = x;
		this->y = y;
	}

	friend Vector2D& operator+(const Vector2D& lhs, const Vector2D& rhs) {
		Vector2D out;
		out.x = lhs.x + rhs.x;
		out.y = lhs.y + rhs.y;
		return out;
	}

	friend Vector2D& operator-(const Vector2D& lhs, const Vector2D& rhs) {
		Vector2D out;
		out.x = lhs.x - rhs.x;
		out.y = lhs.y - rhs.y;
		return out;
	}

	Vector2D& operator+=(const Vector2D& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}

	Vector2D& operator-=(const Vector2D& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}

	Vector2D& operator*(const T rhs) {
		Vector2D out;
		out.x = this->x*rhs;
		out.y = this->y * rhs;

		return out;
	}
};