#pragma once
#include "GameObject.h"
#include "Vector2D.h"

/// Keeps track of the object's position, velocity and acceleration.
class MovementHandler : public GameComponent {
public:

	Vector2D<double> position;
	Vector2D<double> velocity;
	Vector2D<double> acceleration;
	/// Used for maintaining a fixed velocity for the bubbles
	Vector2D<double> baseVelocity;

	MovementHandler(double posX, double posY) {
		position.x = posX;
		position.y = posY;
	};

	MovementHandler(double posX, double posY, double velocityX, double velocityY) {
		position.x = posX;
		position.y = posY;
		velocity.x = velocityX;
		velocity.y = velocityY;
	};

	MovementHandler(double posX, double posY, double velocityX, double velocityY, double accelX, double accelY) {
		position.x = posX;
		position.y = posY;
		velocity.x = velocityX;
		velocity.y = velocityY;
		acceleration.x = accelX;
		acceleration.y = accelY;
	};

	/// Update the velocity and position.
	void update() override {
		velocity += acceleration;
		position += velocity;

		owner->render_rect.x = (int)position.x;
		owner->render_rect.y = (int)position.y;

		//std::cout << "pos(" << position.x << ", " << position.y << ")\n";
	}

	void setPosition(double x, double y) {
		position.x = x;
		position.y = y;
	}

	void setVelocity(double x, double y) {
		velocity.x = x;
		velocity.y = y;
	}

	void setAcceleration(double x, double y) {
		acceleration.x = x;
		acceleration.y = y;
	}
};