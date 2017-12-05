#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "CollisionChecks.h"

class CollisionHandler : public GameComponent {
public:
	MovementHandler * mover;
	SDL_Rect * playZone;
	SDL_Rect * objectRect;

	/// Used to determine if the velocity should be adjusted upon collision, in the case of bubbles but not the player.
	bool adjust_velocity = false;
	
	CollisionHandler(SDL_Rect * playZone, bool velocity) {
		this->playZone = playZone;
		adjust_velocity = velocity;
	}

	/// Set the object pointers from the owner.
	void init() {
		mover = owner->getComponent<MovementHandler>();
		objectRect = &owner->render_rect;
	}

	/// Check if the current object's position intersects with the playerZone, then sets its position and velocity if appropriate.
	void update() {

		if (objectRect->x < playZone->x) {
			mover->position.x = (float) playZone->x + 1;
			if(adjust_velocity) mover->velocity.x = mover->velocity.x * -1;
		}
		if (objectRect->x + objectRect->w > playZone->x + playZone->w) {
			mover->position.x = (float)playZone->x + (float)playZone->w - 1 - objectRect->w;
			if (adjust_velocity) mover->velocity.x = mover->velocity.x * -1;
		}
		if (objectRect->y < playZone->y) {
			mover->position.y = (float)playZone->y + 1;
			if (adjust_velocity) mover->velocity.y = mover->velocity.y * -1;
		}
		if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
			mover->position.y = (float)playZone->y + (float)playZone->h -1 - objectRect->h;
			if (adjust_velocity) mover->velocity.y = mover->velocity.y * -1;
		}
		objectRect->x = (int)mover->position.x;
		objectRect->y = (int)mover->position.y;
	}
};
