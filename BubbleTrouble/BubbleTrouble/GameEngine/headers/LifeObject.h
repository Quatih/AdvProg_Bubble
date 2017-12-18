#pragma once
#include "GameObject.h"

/// LifeObject class, only for objecTmanager usage.
class LifeObject : public GameObject {
public:
	LifeObject() : GameObject(Object_Lives){}
};