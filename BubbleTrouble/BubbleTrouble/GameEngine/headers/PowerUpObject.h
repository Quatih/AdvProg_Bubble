#pragma once
#include "GameObject.h"
#include "MillisTimer.h"
#include <cmath>

enum PowerUpType : std::size_t { PU_Life, PU_Coin };

class PowerUpObject : public GameObject {
public:
	PowerUpType powerUpType;
	MillisTimer powerUpTimer;

	PowerUpObject(PowerUpType type) : GameObject(Object_PowerUp) {
		powerUpType = type;
		std::string path;
		switch (powerUpType) {
		case PU_Life:
			addComponent<TileHandler>("assets/heart.png", 0.5);
			break;
		case PU_Coin:
			addComponent<TileHandler>("assets/coin.png", 0.5);
			break;
		default:
			break;
		}
		addComponent<MovementHandler>(0.0, 0.0, 0.0, 3.0 ,0.0, 0.0);
		addComponent<CollisionHandler>();
		powerUpTimer.start();
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
		if (powerUpTimer.getMillis() >= 3000) this->destroy();

	}

	
};
