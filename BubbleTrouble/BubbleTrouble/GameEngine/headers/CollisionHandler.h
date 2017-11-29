#pragma once
#include "GameObject.h"
#include <iostream>
#include <cmath>

class CollisionHandler : public GameComponent {
public:
	virtual ~CollisionHandler() = default;

	CollisionHandler() {
		std::cout << "new collison object\n";
	}

	void init() override {

	}

	bool collidesWithCirlce(const SDL_Rect & other) {
		int cx = other.x + other.w / 2;
		int cy = other.y + other.h / 2;
		int x = owner->render_rect.x;
		int y = owner->render_rect.y;
		int h = owner->render_rect.h;
		int w = owner->render_rect.w;

		for (int j = y; j <= y + h; j += h / 10) {
			if (sqrt(abs(x - cx)*abs(x - cx) + abs(j - cy)*abs(j - cy)) < (double) other.h / 2 || 
				sqrt(abs(x + w - cx)*abs(x + w - cx) + abs(j - cy)*abs(j - cy)) < (double) other.h / 2) {
				return true;
			}
		}
		for (int i = x; i <= x + w; i += w / 10) {
			if (sqrt(abs(i - cx)*abs(i - cx) + abs(y - cy)*abs(y - cy)) < (double) other.h / 2 || 
				sqrt(abs(i - cx)*abs(i - cx) + abs(y + h - cy)*abs(y + h - cy)) < (double) other.h / 2) {
				return true;
			}
		}
		return false;
	}

	bool collidesWithRect(const SDL_Rect & other) {
		if (owner->render_rect.x + owner->render_rect.w >= other.x &&
			other.x + other.w >= owner->render_rect.x &&
			owner->render_rect.y + owner->render_rect.h >= other.y &&
			other.y + other.h >= owner->render_rect.y) {
			return true;
		}
		return false;
	}

	void update() override {
	}
};
