#pragma once
#include "GameObject.h"

/// Deals wth the Explosion object, only for showing the image for X frames
class ExplosionImageObject : public GameObject {
private:
	const int visibleFrames = 5;
	int renderedFrames = 0;
public:

	ExplosionImageObject() : GameObject(Object_ExplosiveImage) {	}

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
