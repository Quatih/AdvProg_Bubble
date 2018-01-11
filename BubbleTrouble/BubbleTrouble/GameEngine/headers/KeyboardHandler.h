#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "SoundHandler.h"
#include <cmath>


/// Handles all keyboard input for the player
class KeyboardHandler : public GameComponent {
public:
	GameObject * spike;
	MovementHandler *movement;
	double velocity;
	bool freedom;

	std::vector<Uint8> MOVELEFT;
	std::vector<Uint8> MOVERIGHT;
	std::vector<Uint8> SHOOTSPIKE;
	std::vector<Uint8> MOVEUP;
	std::vector<Uint8> MOVEDOWN;

	KeyboardHandler(double velocity, bool freedom, GameObject * spike, PlayerNumber player) {
		this->velocity = velocity;
		this->freedom = freedom;
		this->spike = spike;

		switch (player) {
		case SINGLEPLAYER:
			MOVELEFT.push_back(SDL_SCANCODE_LEFT);
			MOVERIGHT.push_back(SDL_SCANCODE_RIGHT);
			SHOOTSPIKE.push_back(SDL_SCANCODE_RSHIFT);
			MOVEUP.push_back(SDL_SCANCODE_UP);
			MOVEDOWN.push_back(SDL_SCANCODE_DOWN);
			[[fallthrough]];
		case PLAYER1:
			MOVELEFT.push_back(SDL_SCANCODE_A);
			MOVERIGHT.push_back(SDL_SCANCODE_D);
			SHOOTSPIKE.push_back(SDL_SCANCODE_SPACE);
			MOVEUP.push_back(SDL_SCANCODE_W);
			MOVEDOWN.push_back(SDL_SCANCODE_S);
			break;
		case PLAYER2:
			MOVELEFT.push_back(SDL_SCANCODE_LEFT);
			MOVERIGHT.push_back(SDL_SCANCODE_RIGHT);
			SHOOTSPIKE.push_back(SDL_SCANCODE_RSHIFT);
			MOVEUP.push_back(SDL_SCANCODE_UP);
			MOVEDOWN.push_back(SDL_SCANCODE_DOWN);
			break;
		default:
			break;
		}

		std::cout << "Keyboard init\n";
	}

	void init() override {
		movement = owner->getComponent<MovementHandler>();
	}

	/// Checks the current key states and handles the different cases.
	void update() override {
		// Get the key states from the SDL Events
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		bool left = false;
		bool right = false;

		for (auto moveleft : MOVELEFT) {
			if (currentKeyStates[moveleft]) left = true;
		}
		for (auto moveright : MOVERIGHT) {
			if (currentKeyStates[moveright]) right = true;
		}


		if (left && right) {
			movement->velocity.x = 0;
		}
		else if (left) {
			movement->velocity.x = -1 * velocity;
		}
		else if (right) {
			movement->velocity.x = velocity;
		}
		else {
			movement->velocity.x = 0;
		}


		bool spikeshoot = false;
		for (auto shoot : SHOOTSPIKE) {
			if (currentKeyStates[shoot]) spikeshoot = true;
		}

		// Turn spike on and change its position to the player's position.
		if (spikeshoot && !spike->isVisible()) {			
			spike->getComponent<SoundHandler>()->play();

			spike->show();
			spike->render_rect.x = owner->render_rect.x + owner->render_rect.w / 2 - spike->render_rect.w / 2 + 5;
			spike->render_rect.y = owner->render_rect.y;

			spike->getComponent<MovementHandler>()->position.x = (double)spike->render_rect.x;
			spike->getComponent<MovementHandler>()->position.y = (double)spike->render_rect.y;
		}

		bool down = false;
		bool up = false;

		for (auto movedown : MOVEDOWN) {
			if (currentKeyStates[movedown]) down = true;
		}
		for (auto moveup : MOVEUP) {
			if (currentKeyStates[moveup]) up = true;
		}

		// For testing purposes, allows up and down movement
		if (freedom) {
			if (up && down) {
				movement->velocity.y = 0;
			}
			else if (up) {
				movement->velocity.y = -1 * velocity;
			}
			else if (left) {
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
