#pragma once
#include "GameObject.h"
#include "Components.h"

/// SpikeObject class, only for objecTmanager usage.
class SpikeObject : public GameObject {
public:
	SpikeObject() : GameObject(Object_Spike) {
		addComponent<MovementHandler>(0.0, 0.0, 0.0, -6, 0.0, 0.0);
		addComponent<TileHandler>("assets/spike4.png", 1.0);
		addComponent<CollisionHandler>();
		addComponent<SoundHandler>("assets/shoot.wav");
		init();
	}
};