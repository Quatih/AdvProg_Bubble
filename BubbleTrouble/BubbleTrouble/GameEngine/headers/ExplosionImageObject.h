#pragma once
#include "GameObject.h"

class ExplosionImageObject : public GameObject {
public:

	ExplosionImageObject() : GameObject(Object_ExplosiveImage) {	}

	//void update() override {
	//	if (isValid()) {
	//		if (render_rect.y < playZone.y) {
	//			destroy();
	//		}
	//	}
	//}

}; 
