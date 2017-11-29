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


};