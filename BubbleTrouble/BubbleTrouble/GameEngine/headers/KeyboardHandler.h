#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "TileHandler.h"
#include <cmath>

class KeyboardHandler : public GameComponent {
public:
	GameObject * spike;
	MovementHandler *movement;
	SDL_Event *events;
	float velocity;
	bool freedom;
	//bool keydown[256] = { false };

	KeyboardHandler(SDL_Event *events, float velocity, bool freedom, GameObject * spike) {
		this->events = events;
		this->velocity = velocity;
		this->freedom = freedom;
		this->spike = spike;
		std::cout << "Keyboard init\n";

	}

	void init() override {
		movement = owner->getComponent<MovementHandler>();
	}

	void update() override {


		//if (events->type == SDL_KEYDOWN)
		//{
		//	if (events->key.keysym.sym == SDLK_a) {
		//		keydown[SDLK_a] = true;
		//	}
		//	if (events->key.keysym.sym == SDLK_d) {

		//		keydown[SDLK_d] = true;
		//	}
		//	if (events->key.keysym.sym == SDLK_w) {
		//		keydown[SDLK_w] = true;
		//	}
		//	if (events->key.keysym.sym == SDLK_s) {

		//		keydown[SDLK_s] = true;
		//	}
		//	if (events->key.keysym.sym == SDLK_SPACE) {
		//		keydown[SDLK_SPACE] = true;
		//	}
		//}
		//else if (events->type == SDL_KEYUP)
		//{
		//	if (events->key.keysym.sym == SDLK_a) {
		//		keydown[SDLK_a] = false;
		//	}
		//	if (events->key.keysym.sym == SDLK_d) {
		//		keydown[SDLK_d] = false;
		//	}
		//	if (events->key.keysym.sym == SDLK_w) {
		//		keydown[SDLK_w] = false;
		//	}
		//	if (events->key.keysym.sym == SDLK_s) {
		//		keydown[SDLK_s] = false;
		//	}
		//	if (events->key.keysym.sym == SDLK_SPACE) {
		//		keydown[SDLK_SPACE] = false;
		//	}
		//}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if ((currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) && (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT])) {
			movement->velocity.x = 0;
		}
		else if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
			movement->velocity.x = -1 * velocity;
		}
		else if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
			movement->velocity.x = 1 * velocity;
		}
		else {
			movement->velocity.x = 0;
		}

		/// Turn spike on and change its position to the player's position.
		if (currentKeyStates[SDL_SCANCODE_SPACE] && !spike->isValid()) {
			spike->setValid();
			spike->render_rect.x = owner->render_rect.x + owner->render_rect.w / 2 - spike->render_rect.w/2;
			spike->render_rect.y = owner->render_rect.y + owner->render_rect.h / 2;

			spike->getComponent<MovementHandler>()->position.x = (float)spike->render_rect.x;
			spike->getComponent<MovementHandler>()->position.y = (float)spike->render_rect.y;
		}

		/// For testing purposes, allows up and down movement
		if (freedom) {
			if (currentKeyStates[SDL_SCANCODE_W] && currentKeyStates[SDL_SCANCODE_S]) {
				movement->velocity.y = 0;
			}
			else if (currentKeyStates[SDL_SCANCODE_W]) {
				movement->velocity.y = -1*velocity;
			}
			else if (currentKeyStates[SDL_SCANCODE_S]) {
				movement->velocity.y = 1*velocity;
			}
			else {
				movement->velocity.y = 0;
			}
		}
	}
};
