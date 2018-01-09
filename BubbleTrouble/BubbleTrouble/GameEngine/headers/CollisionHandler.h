#pragma once
#include "GameObject.h"

/// Handles collision of the object to a specific zone rectangle, playZone.
class CollisionHandler : public GameComponent {
public:
	MovementHandler * mover;
	SDL_Rect * playZone;
	SDL_Rect * objectRect;

	CollisionHandler(SDL_Rect * playZone) {
		this->playZone = playZone;
	}

	/// Set the object pointers from the owner.
	void init() {
		mover = owner->getComponent<MovementHandler>();
		objectRect = &owner->render_rect;
	}

	/// Check if the current object's position intersects with the playerZone, then sets its position and velocity if appropriate.
	void update() {

		switch (owner->type) {

		case Object_Bubble:
			if (objectRect->x < playZone->x) {
				mover->velocity.x *= -1;
			}
			if (objectRect->x + objectRect->w > playZone->x + playZone->w) {
				mover->velocity.x *= -1;
			}

			if (objectRect->y < playZone->y) {
				mover->velocity.y = mover->baseVelocity.y;
			}
			if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
				mover->velocity.y = mover->baseVelocity.y*-1;
			}
			[[fallthrough]]; // Indicates that the next case statement will also be executed, and that it is intentional.

		case Object_Player:
			if (objectRect->x < playZone->x) {
				mover->position.x = (double)playZone->x;
			}
			if (objectRect->x + objectRect->w > playZone->x + playZone->w) {
				mover->position.x = (double)(playZone->x + playZone->w - objectRect->w);
			}

			if (objectRect->y < playZone->y) {
				mover->position.y = (double)playZone->y;
			}
			if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
				mover->position.y = (double)(playZone->y + playZone->h - objectRect->h);
			}

			objectRect->x = (int)mover->position.x;
			objectRect->y = (int)mover->position.y;
			break;
		case Object_Spike:
			if (objectRect->y < playZone->y) {
				owner->hide();
			}
			break;
		case Object_Explosion:
			break;
		case Object_PowerUp:
			if (objectRect->x < playZone->x) {
				mover->velocity.x *= -1;
			}
			if (objectRect->x + objectRect->w > playZone->x + playZone->w) {
				mover->velocity.x *= -1;
			}

			if (objectRect->y < playZone->y) {
				mover->velocity.y = mover->baseVelocity.y;
			}
			if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
				mover->velocity.y = mover->baseVelocity.y*-1;
			}
			break;
		default:
			break;
		}
	}
};
