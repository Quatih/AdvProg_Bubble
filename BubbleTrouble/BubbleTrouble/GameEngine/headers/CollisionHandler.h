#pragma once
#include "GameComponent.h"
#include <iostream>
class CollisionHandler : public GameComponent {
public:
	virtual ~CollisionHandler() = default;

	CollisionHandler() {
		std::cout << "new collison object\n";
	}
	 virtual void init() override;
	 virtual void update() override;
	 virtual void draw() override;
};
