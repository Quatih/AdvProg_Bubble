#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "Vector2D.h"

class PhysicsHandler : public GameComponent {
public:
	Vector2D<float> * vect;
	
};