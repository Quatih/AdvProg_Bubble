#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include "CollisionChecks.h"
class CollisionHandler : public GameComponent {
public:
	MovementHandler * mover;
	SDL_Rect playZone;
	SDL_Rect * object;

	CollisionHandler(int x, int y, int w, int h) {

		//owner->getComponent<MovementHandler>();
		//object = &(owner->render_rect);
		playZone.x = x;
		playZone.y = y;
		playZone.w = w;
		playZone.h = h;
	}

	void init() {
		mover = owner->getComponent<MovementHandler>();
		object = &owner->render_rect;
	}

	void update() {
		
		if (object->x <= playZone.x) {
			mover->position.x = playZone.x + 1;
			mover->velocity.x = mover->velocity.x * -1;
		}
		if (object->x + object->w >= playZone.x + playZone.w) {
			mover->position.x = playZone.x + playZone.w - 1 - object->w;
			mover->velocity.x = mover->velocity.x * -1;
		}
		if (object->y <= playZone.y) {
			mover->position.y = playZone.y + 1;
			mover->velocity.y = mover->velocity.y * -1;
		}
		if (object->y + object->h >= playZone.y + playZone.h) {
			mover->position.y = playZone.y + playZone.h -1 - object->h;
			mover->velocity.y = mover->velocity.y * -1;
		}
		object->x = (int)mover->position.x;
		object->y = (int)mover->position.y;
	}
};
