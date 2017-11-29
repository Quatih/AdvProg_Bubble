#pragma once
#include "GameObject.h"
#include "Vector2D.h"
#include "PhysicsHandler.h"

class MovementHandler : public GameComponent {
public:
	Vector2D<float> position;
	Vector2D<float> velocity;
	Vector2D<float> acceleration;
	const float framedelay = 1 / 60;

	MovementHandler(float posX, float posY) {
		position.x = posX;
		position.y = posY;
		std::cout << "MovementHandler init\n";
	};

	MovementHandler(float posX, float posY, float velocityX, float velocityY) {
		position.x = posX;
		position.y = posY;
		velocity.x = velocityX;
		velocity.y = velocityY;
		std::cout << "MovementHandler init\n";
	};

	MovementHandler(float posX, float posY, float velocityX, float velocityY, float accelX, float accelY) {
		position.x = posX;
		position.y = posY;
		velocity.x = velocityX;
		velocity.y = velocityY;
		acceleration.x = accelX;
		acceleration.y = accelY;
		std::cout << "MovementHandler init\n";
	};

	void update() override {
		velocity += acceleration;
		position += velocity;

		owner->render_rect.x = (int)position.x;
		owner->render_rect.y = (int)position.y;

		//std::cout << "pos(" << position.x << ", " << position.y << ")\n";
	}

	void setVelocity(float x, float y) {
		velocity.x = x;
		velocity.y = y;
	}

};