#pragma once
#include "GameObject.h"

class PlayerObject : public GameObject {
public:
	int score = 0;

	///Specific collider for the player in order to compensate for whitespace in the image
	PlayerObject() : GameObject(Object_Player) {

	}

	void init() override {
		score = 0;
		for (auto& comps : components) {
			comps->init();
		}
	}

	void update() override {
		if (isValid()) {
			for (auto& comps : components) {
				comps->update();
			}
		}
	}
};