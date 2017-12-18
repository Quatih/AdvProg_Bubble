#pragma once
#include "GameObject.h"


/// SpikeObject class, only for objecTmanager usage.
class SpikeObject : public GameObject {
public:
	SpikeObject() : GameObject(Object_Spike) {	}

};