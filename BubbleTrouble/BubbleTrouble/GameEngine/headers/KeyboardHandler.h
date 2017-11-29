#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "TileHandler.h"

class KeyboardHandler : public GameComponent {
public:
	MovementHandler *movement;
	SDL_Event *events;
	bool keydown[256] = { false };
	KeyboardHandler(SDL_Event *events) {
		this->events = events;
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
		}
		else if (events->type == SDL_KEYUP)
		{
			if (events->key.keysym.sym == SDLK_a) {
				keydown[SDLK_a] = false;
			}
			if (events->key.keysym.sym == SDLK_d) {
				keydown[SDLK_d] = false;
		
			}
		}

		if (keydown[SDLK_a] && keydown[SDLK_d]) {
			movement->velocity->x = 0;
		}
		else if (keydown[SDLK_a]) {
			movement->velocity->x = -1;
		}
		else if (keydown[SDLK_d]) {
			movement->velocity->x = 1;
		}
		else {
			movement->velocity->x = 0;
		}
	}
};
