#pragma once
#include "GameComponent.h"
#include "Components.h"
#include <vector>
#include <algorithm>
#include <array>
#include <memory>



const std::size_t maxComponents = 10;

class GameObject {
private:
	std::array<GameComponent*, maxComponents> components;
	bool valid;

public:
	
	GameObject();
	~GameObject();

	void update();
	bool isValid() const { return valid; };
	void destroy() { valid = false; };

	template <typename T, typename... Ts>
	T& addComponent(Ts&&... args);

	template <typename T> T& getComponent();
};