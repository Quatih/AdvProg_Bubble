#include "../headers/GameObject.h"


GameObject::GameObject() {

	pops = 0;
	valid = true;
}

GameObject::~GameObject() {

	components.clear();
}

void GameObject::update() {
	if (isValid()) {
		for (auto & comps : components) {
			comps->update();
		}
	}
}

void GameObject::init() {
	std::cout << "Init components\n";
	for (auto & comps : components) {
		comps->init();
	}
}


void GameObject::draw() {
	if (isValid()) {
		for (auto& comps : components) {
			comps->draw();
		}
	}
}