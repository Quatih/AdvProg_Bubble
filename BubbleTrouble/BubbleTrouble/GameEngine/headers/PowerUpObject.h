#pragma once
#include "GameObject.h"
#include <cmath>

class PowerUpObject : public GameObject {
public:

	PowerUpObject():GameObject(Object_PowerUp) {
		std::cout << "poweerUp object created";
	}

	PowerUpObject(SDL_Renderer * renderer, SDL_Rect* playZone, std::string path) : GameObject(Object_PowerUp) {
		addComponent<TileHandler>(renderer, path.c_str(),0.3f);
		addComponent<MovementHandler>(0.0, 0.0,0.0,2.0f,0.0,0.0);
		addComponent<CollisionHandler>(playZone);
		init();
	}


	~PowerUpObject() {
		
		
	}


	void update() override {
		if (isVisible()) {
				for (auto& comps : components) {
					comps->update();
			}
		}

	}

	
};
