#pragma once
#include "GameObject.h"
#include "Vector2D.h"
#include "PhysicsHandler.h"

class MovementHandler : public GameComponent {
public:
	Vector2D<float> position;
	Vector2D<float> velocity;

	MovementHandler(float x, float y) {
		position.x = x;
		position.y = y;
		std::cout << "MovementHandler init\n";
	};

	~MovementHandler() {
	}

	void update() override {
		position.x = position.x + velocity.x;
		position.y = position.y + velocity.y;
		owner->render_rect.x = (int)position.x;
		owner->render_rect.y = (int)position.y;

		std::cout << "pos(" << position.x << ", " << position.y << ")\n";
	}

	void setVelocity(float x, float y) {
		velocity.x = x;
		velocity.y = y;
	}

	void init() override {

	}
	void draw() override {

	}
};