#pragma once
#include "GameComponent.h"
#include "Components.h"
#include <vector>


class GameObject {
public:
	/// Keep unique IDs for different component types.
	
	std::vector<GameComponent> components;


	bool enabled = true;
	
	template <typename T, typename... Ts>
	T& addComponent(Ts&&... args);

	void update();
};