#pragma once
#include "GameObject.h"

class ExplosionImageObject : public GameObject {
private:
	const int visibleFrames = 5;
	int renderedFrames = 0;
public:

	ExplosionImageObject() : GameObject(Object_ExplosiveImage) {	}

	void update() override {
		if (isValid()) {
			if (renderedFrames < visibleFrames) {
				for (auto& comps : components) {
					comps->update();
				}
				renderedFrames++;
			}
			else {
				renderedFrames = 0;
				destroy();
			}
		}
	}

	void draw() override {
		if (isValid()) {
			for (auto& comps : components) {
				comps->draw();
			}
		}
	}

}; 
