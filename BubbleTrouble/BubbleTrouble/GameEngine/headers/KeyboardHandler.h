#pragma once
#include "GameComponent.h"

class KeyboardHandler : public GameComponent {
public:
	KeyboardHandler() {

	}
	void init() override;

	void update() override;
};
