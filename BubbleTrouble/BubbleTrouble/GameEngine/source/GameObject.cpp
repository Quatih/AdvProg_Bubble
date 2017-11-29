#include "../headers/GameObject.h"


GameObject::GameObject(int height, int width, int posX, int posY) {
	default_rect.h = 32;
	default_rect.w = 32;
	default_rect.x = 0;
	default_rect.y = 0;
	render_rect.h = height;
	render_rect.w = width;
	render_rect.x = posX;
	render_rect.y = posY;


	valid = true;
}

GameObject::~GameObject() {
	//for (GameComponent * comps : componentArray) {
	//	delete comps;
	//}
	components.clear();
}

void GameObject::update() {
	for (auto & comps : components) {
		comps->update();
	}
}

void GameObject::draw() {
	for (auto& comps : components) {
		comps->draw();
	}
}