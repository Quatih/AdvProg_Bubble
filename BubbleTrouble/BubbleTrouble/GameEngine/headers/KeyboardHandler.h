#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "TileHandler.h"
#include <cmath>

class KeyboardHandler : public GameComponent {
public:
	MovementHandler *movement;
	SDL_Event *events;
	float velocity;
	bool yfreedom;
	bool keydown[256] = { false };
	KeyboardHandler(SDL_Event *events, float velocity, bool freedom) {
		this->events = events;
		this->velocity = velocity;
		yfreedom = freedom;
		std::cout << "Keyboard init\n";

	}


	void init() override {
		movement = owner->getComponent<MovementHandler>();
	}

	void update() override {

		if (events->type == SDL_KEYDOWN)
		{
			if (events->key.keysym.sym == SDLK_a) {
				keydown[SDLK_a] = true;
			}
			if (events->key.keysym.sym == SDLK_d) {

				keydown[SDLK_d] = true;
			}
			if (events->key.keysym.sym == SDLK_w) {
				keydown[SDLK_w] = true;
			}
			if (events->key.keysym.sym == SDLK_s) {

				keydown[SDLK_s] = true;
			}
			if (events->key.keysym.sym == SDLK_SPACE) {

			}
		}
		else if (events->type == SDL_KEYUP)
		{
			if (events->key.keysym.sym == SDLK_a) {
				keydown[SDLK_a] = false;
			}
			if (events->key.keysym.sym == SDLK_d) {
				keydown[SDLK_d] = false;
			}
			if (events->key.keysym.sym == SDLK_w) {
				keydown[SDLK_w] = false;
			}
			if (events->key.keysym.sym == SDLK_s) {
				keydown[SDLK_s] = false;
			}
		}

		if (keydown[SDLK_a] && keydown[SDLK_d]) {
			movement->velocity.x = 0;
		}
		else if (keydown[SDLK_a]) {
			movement->velocity.x = -1 * velocity;
		}
		else if (keydown[SDLK_d]) {
			movement->velocity.x = 1 * velocity;
		}
		else {
			movement->velocity.x = 0;
		}


		if (yfreedom) {
			if (keydown[SDLK_w] && keydown[SDLK_s]) {

				movement->velocity.y = 0;
			}
			else if (keydown[SDLK_w]) {
				movement->velocity.y = -1*velocity;
			}
			else if (keydown[SDLK_s]) {
				movement->velocity.y = 1*velocity;
			}
			else {
				movement->velocity.y = 0;
			}
		}
	}
};
