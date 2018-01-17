#pragma once
#include "GameObject.h"


/// Handles score for the player object
class PlayerObject : public GameObject {
public:
	PlayerNumber playerType;
	///Specific collider for the player in order to compensate for whitespace in the image
	PlayerObject(GameObject * spike, PlayerNumber playerNumber) : GameObject(Object_Player) {
		addComponent<KeyboardHandler>(3.8, false, spike, playerNumber);
		addComponent<MovementHandler>(0, 0);
		addComponent<TileHandler>("assets/duder4.png", 0.9);
		addComponent<CollisionHandler>();
		addComponent<SoundHandler>("assets/hit.wav");
		init();
		render_rect.y = playZone.y + playZone.h - render_rect.h;
		playerType = playerNumber;
		switch (playerNumber) {
		case SINGLEPLAYER:
			render_rect.x = playZone.x + playZone.w / 2 - render_rect.w / 2 - 5;
			break;
		case PLAYER1:
			render_rect.x = playZone.x + playZone.w / 4 - render_rect.w / 2 - 5;
			break;
		case PLAYER2:
			render_rect.x = playZone.x + 3*playZone.w / 4 - render_rect.w / 2 - 5;
			break;
		default:
			break;
		}

		getComponent<MovementHandler>()->setPosition(render_rect.x, render_rect.y);
		getComponent<MovementHandler>()->setAcceleration(0, Base_BubbleY_acceleration);


	}

};