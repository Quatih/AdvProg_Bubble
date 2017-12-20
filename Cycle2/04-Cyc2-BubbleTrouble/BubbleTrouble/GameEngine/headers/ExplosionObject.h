#pragma once
#include "GameObject.h"

/// Deals wth the Explosion object, only for showing the image for X frames
class ExplosionObject : public GameObject {
private:
	const int visibleFrames = 6;
	int renderedFrames = 0;
public:

	ExplosionObject() : GameObject(Object_Explosion) {	}

	void update() override {
		if (isVisible()) {
			if (renderedFrames < visibleFrames) {
				for (auto& comps : components) {
					comps->update();
				}
				renderedFrames++;
			}
			else {
				renderedFrames = 0;
				hide();
			}
		}
	}
}; 
