#include "../headers/GameObject.h"


GameObject::GameObject() {
	std::cout << "Gameobject created\n";
	valid = true;
}

GameObject::~GameObject() {
	for (GameComponent * comps : components) {
		delete comps;
	}
}

void GameObject::update() {
	for (GameComponent * comps : components) {
		comps->update();
	}
}

void GameObject::draw() {
	for (GameComponent * comps : components) {
		comps->draw();
	}
}