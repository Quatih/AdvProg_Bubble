#pragma once
#include "GameComponent.h"
#include "Vector2D.h"
#include "PhysicsHandler.h"

class MovementHandler : public GameComponent {
public:
	Vector2D<float> * position;
	Vector2D<float> * velocity;

	MovementHandler(float x, float y) {
		position = new Vector2D <float>(x, y);
		velocity = new Vector2D <float>();
	};

	MovementHandler() {
		delete position;
		delete velocity;
	}

	void update() override {
		position->x = position->x + velocity->x;
		position->y = position->y + velocity->y;
	}

	void setVelocity(float x, float y) {
		velocity->x = x;
		velocity->y = y;
	}

	void init() override {

	}
};