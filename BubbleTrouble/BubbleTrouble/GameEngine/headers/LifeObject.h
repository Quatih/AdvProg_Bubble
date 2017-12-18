#pragma once
#include "GameObject.h"

/// LifeObject class
class LifeObject : public GameObject {
public:
	LifeObject() : GameObject(Object_Lives){}
};