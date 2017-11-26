#include "../headers/GameObject.h"


std::size_t getUniqueID() {
	/// Maintains value during runtime
	static size_t ID = 0;
	return ID++;
}

template <typename T> std::size_t getComponentID() {
	static ID = getUniqueID();
	return ID;
}


GameObject::GameObject() {
	std::cout << "Gameobject created";
	valid = true;
}

GameObject::~GameObject() {
	for (GameComponent * comps : components) {
		delete comps;
	}
}

/// Add component of type T with arguments Ts to this GameObject
template <typename T, typename... Ts>
T& GameObject::addComponent(Ts&&... args) {

	/// Do this to forward arguments made to addcomponent to the newly created component
	T* comp = new T(std::forward<Ts>(args)...);
	comp->owner = this;
	components[getComponentID<T>()] = comp;
	comp->init();

	return *comp;
}

void GameObject::update() {
	for (auto& comps : components) {
		comps->update();
	}
}



template <typename T> T& GameObject::getComponent() {
	return *dynamic_cast<T*>(components[getCompnentId<T>()]);
}