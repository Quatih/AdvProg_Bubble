#pragma once

template<typename T> class Vector2D {
public:
	T x;
	T y;

	Vector2D() {
		x = 0;
		y = 0;
	}

	Vector2D(T x, T y) {
		this->x = x;
		this->y = y;
	}


	friend Vector2D& operator+(Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D out;
		out.x = lhs.x + rhs.x;
		out.y = lhs.y + rhs.y;
		return out;
	}
	friend Vector2D& operator-(Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D out;
		out.x = lhs.x - rhs.x;
		out.y = lhs.y - rhs.y;
		return out;
	}
	Vector2D& operator+=(const Vector2D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;

		return *this;
	}
	Vector2D& operator-=(const Vector2D& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;

		return *this;
	}

	Vector2D& operator*(const T rhs)
	{
		Vector2D out;
		out.x = this->x*rhs;
		out.y = this->y * rhs;

		return out;
	}
};