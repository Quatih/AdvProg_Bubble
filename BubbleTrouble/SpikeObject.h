#include "GameObject.h"


class SpikeObject : public GameObject {
public:
	SDL_Rect playZone;

	virtual void update() override {
		
		std::cout << "pos: " << getComponent<MovementHandler>()->position.x << ", " << getComponent<MovementHandler>()->position.y << std::endl;
		//If the spike has reached the top, destroy it.
		if (render_rect.y < playZone.y) {
			destroy();
		}

		if (isValid()) {
			for (auto& comps : components) {
				comps->update();
			}
		}
	}

	virtual void init() override {
		for (auto& comps : components) {
			comps->init();
		}
	}

	virtual void draw() override {
		if (isValid()) {
			for (auto& comps : components) {
				comps->draw();
			}
		}
	}

	

};