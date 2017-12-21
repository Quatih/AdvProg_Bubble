#pragma once
#include "GameObject.h"

/// Deals wth the Explosion object, only for showing the image for X frames
class ExplosionObject : public GameObject {
private:
	const int visibleFrames = 6;
	int renderedFrames = 0;
public:

	ExplosionObject(SDL_Renderer * renderer) : GameObject(Object_Explosion) {
		addComponent<TileHandler>(renderer, "assets/collision.png", 0.5);
		addComponent<MovementHandler>(0.0, 0.0);
	}

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
