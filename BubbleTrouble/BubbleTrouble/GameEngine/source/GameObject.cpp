#include "../headers/GameObject.h"




GameObject::GameObject() {
	std::cout << "Gameobject created";
	valid = true;
}

GameObject::~GameObject() {
	for (GameComponent * comps : components) {
		delete comps;
	}
}


void GameObject::update() {
	for (auto& comps : components) {
		comps->update();
	}
}
