#pragma once
#include <cmath>

#ifdef __linux__ 

#include <SDL2/SDL.h>

#else

#include "SDL.h"

#endif

/// Checks the collision bounds of two SDL_Rects and returns true if the rectangles collide.
bool collidesWithRect(const SDL_Rect check, const SDL_Rect other) {
	if (check.x + check.w >= other.x &&
		other.x + other.w >= check.x &&
		check.y + check.h >= other.y &&
		other.y + other.h >= check.y) {
		return true;
	}
	return false;
}


/// Checks the collision bounds of a rectangle and a circle, using the distance from the center of the circle.
bool collidesWithCircle(const SDL_Rect check, const SDL_Rect circle) {
	int cx = circle.x + circle.w / 2;
	int cy = circle.y + circle.h / 2;
	double ch = circle.h;
	int x = check.x;
	int y = check.y;
	int h = check.h;
	int w = check.w;

	/// Only do the circular calculations if the rectangles collide.
	if (collidesWithRect(check, circle)) {
		/// Check for collision to the circle from 10 positions of either sides of the square.

		for (int j = y; j <= y + h; j += h / 10) {
			if (sqrt(abs(x - cx)*abs(x - cx) + abs(j - cy)*abs(j - cy)) <= ch / 2 ||
				sqrt(abs(x + w - cx)*abs(x + w - cx) + abs(j - cy)*abs(j - cy)) <= ch / 2) {
				return true;
			}
		}
		for (int i = x; i <= x + w; i += w / 10) {
			if (sqrt(abs(i - cx)*abs(i - cx) + abs(y - cy)*abs(y - cy)) <= ch / 2 ||
				sqrt(abs(i - cx)*abs(i - cx) + abs(y + h - cy)*abs(y + h - cy)) <= ch / 2) {
				return true;
			}
		}
	}
	return false;
}



