#pragma once
#include "GameObject.h"
#include "MovementHandler.h"


/// Handles collision of the object to a specific zone rectangle, playZone.
class CollisionHandler : public GameComponent {
public:
	MovementHandler * mover;
	SDL_Rect * playZone;
	SDL_Rect * objectRect;

	/// Used to determine if the velocity should be adjusted upon collision, in the case of bubbles but not the player.


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
				mover->velocity.y *= -1;
			}
			if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
				mover->velocity.y *= -1;
			}
			//[[fallthrough]]; // Indicates that the next case statement will also be executed, and that it is intentional.
		case Object_Player:
			if (objectRect->x < playZone->x) {
				mover->position.x = (float)playZone->x;
			}
			if (objectRect->x + objectRect->w > playZone->x + playZone->w) {
				mover->position.x = (float)(playZone->x + playZone->w - objectRect->w);
			}

			if (objectRect->y < playZone->y) {
				mover->position.y = (float)playZone->y;
			}
			if (objectRect->y + objectRect->h > playZone->y + playZone->h) {
				mover->position.y = (float)(playZone->y + playZone->h - objectRect->h);
			}

			objectRect->x = (int)mover->position.x;
			objectRect->y = (int)mover->position.y;
			break;
		case Object_Spike:

			if (objectRect->y < playZone->y) {
				owner->destroy();
			}
			break;
		case Object_Explosive:
				owner->destroy();
			break;
		default:
			break;
		}
	}

	/// Returns true if the object collides with the playerZone.
	//bool collidesWithZone() {
	//	return collidesWithRect(*playZone, *objectRect);
	//}
};
