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

	std::size_t getUniqueID() {
		/// Maintains value during runtime
		static std::size_t ID = 0;
		return ID++;
	}

	/// Purpose is to keep a unique ID to the template component of type T
	template <typename T> std::size_t getComponentID() {
		static std::size_t ID = getUniqueID();
		return ID;
	}

public:

	GameObject();
	~GameObject();

	void update();
	bool isValid() const { return valid; };
	void destroy() { valid = false; };
	void draw();
	/// Add component of type T with arguments Ts to this GameObject
	template <typename T, typename... Ts>
	void addComponent(Ts&&... args)
	{
		/// Forward arguments made to addcomponent to the newly created component
		T* comp = new T(std::forward<Ts>(args)...);
		comp->owner = this;

		/// Add the component to the array at the unique location of this template type
		components[getComponentID<T>()] = comp;
		comp->init();
	}

	template <typename T> T* getComponent() {
		return dynamic_cast<T*>(components[getComponentID<T>()]);
	}
};