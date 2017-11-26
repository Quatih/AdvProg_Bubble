#pragma once
#include "headers\GameObject.h"
#include "headers\GameComponent.h"
#include "headers\Components.h"
#include "headers\Scoreboard.h"
#include "headers\SoundPlayer.h"
#include <vector>
#include "headers\Menu.h"
#include "headers\Map.h"

class GameEngine {
private:
	GameObject * player;
public:

	GameEngine(){
		std::cout << "Constructed.\n";
	};
	~GameEngine(){
		delete player;
	};

	void init() {
		
		player = new GameObject();

		player->addComponent<TileHandler>();
		player->getComponent<TileHandler>().init();
	}
};