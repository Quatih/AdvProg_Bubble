#pragma once
#include "GameObject.h"

class SpikeObject : public GameObject {
public:

	SpikeObject() : GameObject(Object_Spike) {	}

	//void update() override {
	//	if (isValid()) {
	//		if (render_rect.y < playZone.y) {
	//			destroy();
	//		}
	//	}
	//}

};