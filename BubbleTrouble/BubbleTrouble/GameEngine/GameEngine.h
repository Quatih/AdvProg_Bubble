#pragma once
#include "headers\GameObject.h"
#include "headers\GameComponent.h"
#include "headers\Components.h"
#include "headers\Scoreboard.h"
#include "headers\SoundPlayer.h"
#include <vector>
#include "headers\Menu.h"
#include "headers\Map.h"
#include "SDL_image.h"

class GameEngine {
private:
	GameObject * player;
	std::vector<GameObject*> bubbles;
	Map * map;
	SDL_Window * window;


public:
	SDL_Renderer * renderer;
	SDL_Rect srcrect, destrect;
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
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_ShowWindow(window);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		player = new GameObject();
		player->addComponent<TileHandler>();
		player->addComponent<TextureHandler>(renderer);
		player->getComponent<TextureHandler>()->loadTexture("C:/Users/hvati/source/repos/AdvProg_Bubble/BubbleTrouble/BubbleTrouble/assets/enemy.png");
		player->getComponent<TextureHandler>()->loadTexture("assets/weirdguy.png");

		srcrect.h = 128;
		srcrect.w = 128;
		srcrect.x = 0;
		srcrect.y = 0;
		destrect = srcrect;
	}
	void draw() {
		SDL_RenderClear(renderer);

		SDL_RenderCopyEx(renderer, player->getComponent<TextureHandler>()->getTexture(), &srcrect, &destrect, NULL, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);
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