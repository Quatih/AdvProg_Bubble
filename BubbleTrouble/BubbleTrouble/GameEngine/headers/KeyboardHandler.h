#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "TileHandler.h"

class KeyboardHandler : public GameComponent {
public:
	MovementHandler *movement;
	SDL_Event *events;
	KeyboardHandler(SDL_Event *events) {
		this->events = events;
	}


	void init() override {
		movement = owner->getComponent<MovementHandler>();
	}

	void update() override {
		if (events->type == SDL_KEYDOWN)
		{
			switch (events->key.keysym.sym)
			{
			case SDLK_w:
				movement->velocity->y = -1;
				break;
			case SDLK_a:
				movement->velocity->x = -1;
				break;
			case SDLK_d:
				movement->velocity->x = 1;
				break;
			case SDLK_s:
				movement->velocity->y = 1;
				break;
			default:
				break;
			}
		}
		else if (events->type == SDL_KEYUP)
		{
			switch (events->key.keysym.sym)
			{
			case SDLK_w:
				movement->velocity->y = 0;
				break;
			case SDLK_a:
				movement->velocity->x = 0;
				break;
			case SDLK_d:
				movement->velocity->x = 0;
				break;
			case SDLK_s:
				movement->velocity->y = 0;
				break;
			default:
				break;
			}
		}
	}
};
