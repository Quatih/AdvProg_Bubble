#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "CollisionChecks.h"

class CollisionHandler : public GameComponent {
public:
	MovementHandler * mover;
	SDL_Rect * playZone;
	SDL_Rect * object;
	bool adjust_velocity = false;
	CollisionHandler(SDL_Rect * playZone, bool velocity) {
		this->playZone = playZone;
		//owner->getComponent<MovementHandler>();
		//object = &(owner->render_rect);
		adjust_velocity = velocity;
	}

	void init() {
		mover = owner->getComponent<MovementHandler>();
		object = &owner->render_rect;
	}

	void update() {
		/// Check if the current object's postion intersects with the playerZone.

		if (object->x < playZone->x) {
			mover->position.x = (float) playZone->x + 1;
			if(adjust_velocity) mover->velocity.x = mover->velocity.x * -1;
		}
		if (object->x + object->w > playZone->x + playZone->w) {
			mover->position.x = (float)playZone->x + (float)playZone->w - 1 - object->w;
			if (adjust_velocity) mover->velocity.x = mover->velocity.x * -1;
		}
		if (object->y < playZone->y) {
			mover->position.y = (float)playZone->y + 1;
			if (adjust_velocity) mover->velocity.y = mover->velocity.y * -1;
		}
		if (object->y + object->h > playZone->y + playZone->h) {
			mover->position.y = (float)playZone->y + (float)playZone->h -1 - object->h;
			if (adjust_velocity) mover->velocity.y = mover->velocity.y * -1;
		}
		object->x = (int)mover->position.x;
		object->y = (int)mover->position.y;
	}
};
