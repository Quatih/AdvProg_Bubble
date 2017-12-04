#include "../headers/GameObject.h"


GameObject::GameObject(int width, int height, float scale) {
	default_rect.h = height;
	default_rect.w = width;
	default_rect.x = 0;
	default_rect.y = 0;
	render_rect.h = (int)(height*scale);
	render_rect.w = (int)(width*scale);
	render_rect.x = 0;
	render_rect.y = 0;
	pops = 0;
	valid = true;
}

GameObject::~GameObject() {
	for (GameComponent * comps : componentArray) {
		delete comps;
	}
	components.clear();
}

void GameObject::update() {
	if (isValid()) {
		for (auto & comps : components) {
			comps->update();
		}
	}
}

void GameObject::draw() {
	if (isValid()) {
		for (auto& comps : components) {
			comps->draw();
		}
	}
}