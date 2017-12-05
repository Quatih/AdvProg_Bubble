#pragma once
#include <cmath>

#ifdef __linux__ 

#include <SDL2/SDL.h>

#else

#include "SDL.h"

#endif

/// Checks the collision bounds of two SDL_Rects and returns true if the rectangles collide.
bool collidesWithRect(const SDL_Rect check, const SDL_Rect other) {
	if (check.x + check.w >= other.x && other.x + other.w >= check.x &&
		check.y + check.h >= other.y && other.y + other.h >= check.y) {
		return true;
	}
	else{ }
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
		// Check for collision to the circle from either sides of the square.

		for (int j = y; j <= y + h; j += 1) {
			// Calculate the distance from the center of the circle to the position
			double dist = sqrt((x - cx)*(x - cx) + (j - cy)*(j - cy));
			double distw = sqrt((x + w - cx)*(x + w - cx) + (j - cy)*(j - cy));

			if (dist <= ch / 2 || distw <= ch / 2) {
				return true;
			}
		}
		for (int i = x; i <= x + w; i += 1) {
			// Calculate the distance from the center of the circle to the position
			double dist = sqrt((i - cx)*(i - cx) + (y - cy)*(y - cy));
			double disth = sqrt((i - cx)*(i - cx) + (y + h - cy)*(y + h - cy));

			if (dist <= ch / 2 || disth <= ch / 2) {
				return true;
			}
		}
	}
	return false;
}



