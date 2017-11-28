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
	Map * map;
public:
	static SDL_Renderer * Renderer;

	GameEngine(){
		std::cout << "Game Engine Constructed.\n";
	};
	~GameEngine(){
		delete player;
		bubbles.clear();
	};

	void init() {
		player = new GameObject();
		player->addComponent<TileHandler>();
		player->addComponent<Vector2D<float>>(0.1f, 0.2f);
		std::cout << player->getComponent<Vector2D<float>>()->x <<  "  " <<player->getComponent<Vector2D<float>>()->y << std::endl;
	}

	/// Deletes invalidated game objects
	void clean() {
		for (auto bubble = bubbles.begin(); bubble != bubbles.end(); ++bubble) {
			if (!(*bubble)->isValid()) {
				auto thisbubble = bubble;
				--bubble;
				bubbles.erase(thisbubble);
			}
		}
	}
};