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
	SDL_Window * window;
public:
	SDL_Renderer * renderer;

	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag){
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow(title.c_str(), winposx, winposy, winwidth, winheight, flag);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		std::cout << "Game Engine Constructed.\n";
	};

	~GameEngine(){
		delete player;
		bubbles.clear();
		
	};

	void init() {
		SDL_ShowWindow(window);
		player = new GameObject();
		player->addComponent<TileHandler>();

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