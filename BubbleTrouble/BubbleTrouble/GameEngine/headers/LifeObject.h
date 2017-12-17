#pragma once
#include "GameObject.h"

class LifeObject : public GameObject {
public:
	LifeObject() : GameObject(Object_Lives){}
};