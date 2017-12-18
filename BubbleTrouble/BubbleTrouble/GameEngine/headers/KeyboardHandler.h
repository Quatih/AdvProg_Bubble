#pragma once
#include "SpikeObject.h"
#include "MovementHandler.h"
#include "SoundHandler.h"
#include "SDL_mixer.h"
#include <cmath>


/// Handles all keyboard input for the player
class KeyboardHandler : public GameComponent {
public:
	GameObject * spike;
	MovementHandler *movement;
	double velocity;
	bool freedom;

	KeyboardHandler(double velocity, bool freedom, GameObject * spike) {
		this->velocity = velocity;
		this->freedom = freedom;
		this->spike = spike;
		std::cout << "Keyboard init\n";
	}

	void init() override {
		movement = owner->getComponent<MovementHandler>();
	}

	/// Checks the current key states and handles the different cases.
	void update() override {
		// Get the key states from the SDL Events
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		if ((currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) && (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT])) {
			movement->velocity.x = 0;
		}
		else if (currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT]) {
			movement->velocity.x = -1 * velocity;
		}
		else if (currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT]) {
			movement->velocity.x = velocity;
		}
		else {
			movement->velocity.x = 0;
		}

		// Turn spike on and change its position to the player's position.
		if (currentKeyStates[SDL_SCANCODE_SPACE] && !spike->isVisible()) {			
			spike->getComponent<SoundHandler>()->play();

			spike->show();
			spike->render_rect.x = owner->render_rect.x + owner->render_rect.w / 2 - spike->render_rect.w / 2 + 5;
			spike->render_rect.y = owner->render_rect.y;

			spike->getComponent<MovementHandler>()->position.x = (double)spike->render_rect.x;
			spike->getComponent<MovementHandler>()->position.y = (double)spike->render_rect.y;
		}

		// For testing purposes, allows up and down movement
		if (freedom) {
			if ((currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) && (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN])) {
				movement->velocity.y = 0;
			}
			else if (currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP]) {
				movement->velocity.y = -1 * velocity;
			}
			else if (currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN]) {
				movement->velocity.y = velocity;
			}
			else {
				movement->velocity.y = 0;
			}

			// Normalize the speed to the velocity
			if (movement->velocity.y != 0 && movement->velocity.x != 0) {
				movement->velocity.x = sqrt(velocity*velocity / 2) * movement->velocity.x / velocity;
				movement->velocity.y = sqrt(velocity*velocity / 2) * movement->velocity.y / velocity;
			}
		}
	}
};
