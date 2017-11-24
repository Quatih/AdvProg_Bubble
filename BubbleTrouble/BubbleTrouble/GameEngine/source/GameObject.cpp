#include "../headers/GameObject.h"




template <typename T, typename... Ts>
T& GameObject::addComponent(Ts&&... args) {
	T* c(new T(std::forward<Ts>(args)...));
	c->entity = this;
	std::unique_ptr<Component>uPtr{ c };
	components.emplace_back(std::move(uPtr));

	componentArray[getComponentTypeID<T>()] = c;
	componentBitset[getComponentTypeID<T>()] = true;

	c->init();
	return *c;
}

void GameObject::update() {
	for (auto& comps : components) {
		comps.update();
	}
}
