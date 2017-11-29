#pragma once
#include <cmath>
#include "SDL.h"

bool collidesWithRect(const SDL_Rect * check, const SDL_Rect * other) {
	if (check->x + check->w >= other->x &&
		other->x + other->w >= check->x &&
		check->y + check->h >= other->y &&
		other->y + other->h >= check->y) {
		return true;
	}
	return false;
}


bool collidesWithCirlce(const SDL_Rect * check, const SDL_Rect * other) {
	int cx = other->x + other->w / 2;
	int cy = other->y + other->h / 2;
	int x = check->x;
	int y = check->y;
	int h = check->h;
	int w = check->w;
	for (int j = y; j <= y + h; j += h / 10) {
		if (sqrt(abs(x - cx)*abs(x - cx) + abs(j - cy)*abs(j - cy)) <= (double)other->h / 2 ||
			sqrt(abs(x + w - cx)*abs(x + w - cx) + abs(j - cy)*abs(j - cy)) <= (double)other->h / 2) {
			return true;
		}
	}
	for (int i = x; i <= x + w; i += w / 10) {
		if (sqrt(abs(i - cx)*abs(i - cx) + abs(y - cy)*abs(y - cy)) <= (double)other->h / 2 ||
			sqrt(abs(i - cx)*abs(i - cx) + abs(y + h - cy)*abs(y + h - cy)) <= (double)other->h / 2) {
			return true;
		}
	}
	return false;
}



