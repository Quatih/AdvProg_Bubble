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
	std::vector<GameObject*> bubbles;
	std::vector<GameObject*> spikes;
public:
	static SDL_Renderer * Renderer;

	GameEngine(){
		std::cout << "Game Engine Constructed.\n";
	};
	~GameEngine(){
		delete player;
	};

	void init() {
		
		player = new GameObject();

		player->addComponent<TileHandler>();
		player->getComponent<TileHandler>().init();
		auto comp = player->getComponent<TileHandler>();
		comp.init();
	}
};