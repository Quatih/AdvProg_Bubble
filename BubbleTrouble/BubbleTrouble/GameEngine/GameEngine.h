#pragma once
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/TextureLoader.h"
#include <vector>
#include <random>

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#else

#include "SDL.h"
#include "SDL_image.h"

#endif


class GameEngine {
private:
	GameObject * player;
	std::vector<GameObject*> bubbles;
	GameObject * spike;

	SDL_Window * window;
	SDL_Event events;
	bool running;
	SDL_Rect playZone; 
	SDL_Rect spikeZone;

	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };
	std::vector<TextureLoader*> bubbleTextures;

public:

	SDL_Renderer * renderer;

	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag) {
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow(title.c_str(), winposx, winposy, winwidth, winheight, flag);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		std::cout << "Game Engine Constructed.\n";
		running = true;
		playZone.x = 0;
		playZone.y = 0;
		playZone.h = winheight;
		playZone.w = winwidth;

		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	};

	~GameEngine() {
		delete player;
		bubbles.clear();
		running = false;
		delete spike;
		bubbleTextures.clear();
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};

	bool inline isRunning() {
		return running;
	}

	void init() {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_ShowWindow(window);

		player = new GameObject(); 
		spike = new GameObject();

		player->addComponent<KeyboardHandler>(&events, 3.5f, false, spike);
		player->addComponent<MovementHandler>((float)playZone.w / 2, (float)playZone.h);
		player->addComponent<TileHandler>(renderer, "assets/SuperWeird3.png", 1.0f);
		player->addComponent<CollisionHandler>(&playZone, false);
		spike->addComponent<MovementHandler>(0.0f, 0.0f, 0.0f, -4.8f, 0.0f, 0.0f);
		spike->addComponent<TileHandler>(renderer, "assets/spike4.png", 1.0f);

		spike->destroy();
		player->init();
		spike->init();

		for (int i = 0; i < 4; i++) {
			bubbleTextures.push_back(new TextureLoader(renderer, "assets/WhiteBall_128x128.png"));
			bubbleTextures[i]->applyColor(colorarray[i]);
		}
		for (int i = 0; i < 4; i++) {
			generateRandomBubble();
		}
	}

	void update() {
		player->update();
		for (auto bubble : bubbles) {
			bubble->update();
		}
		spike->update();

		for (auto bubble : bubbles) {
			if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
				std::cout << "Collides with bubble\n";
			}
		} 
		if (spike->isValid()) {
			std::vector<GameObject*> tempbubbles;
			/// If the spike has reached the top, destroy it.
			if (spike->render_rect.y <= 1) {
				spike->destroy();
			}
			for (auto bubble : bubbles) {
				if (collidesWithRect((spike->render_rect), (bubble->render_rect))) {
					spike->destroy();
					bubble->destroy();
					std::cout << "Bubble popped\n";
					if (bubble->pops > 0) {
						int cindex = rand() % bubbleTextures.size();
						tempbubbles.push_back(addBubble(bubble->render_rect.h / 3, bubble->render_rect.x, bubble->render_rect.y, bubble->getComponent<MovementHandler>()->velocity.x,
							(float)-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.8), bubble->getComponent<MovementHandler>()->acceleration.y, bubble->pops - 1, bubbleTextures[cindex]));
						tempbubbles.push_back(addBubble(bubble->render_rect.h / 3, bubble->render_rect.x, bubble->render_rect.y, -bubble->getComponent<MovementHandler>()->velocity.x,
							(float)-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.8), bubble->getComponent<MovementHandler>()->acceleration.y, bubble->pops - 1, bubbleTextures[cindex]));
					}
					break; //Break so that we pop only one bubble.
				}
			}

			/// add them in later so that they're not iterated over in the previos loop.
			for (auto bubble : tempbubbles) {
				bubbles.push_back(bubble);
			}
		}
		if (bubbles.empty()) {
			for (int i = 0; i < 5; i++) {
				generateRandomBubble();
			}
		}
	}

	void render() {
		SDL_RenderClear(renderer);
		
		spike->draw();
		player->draw();

		for (auto bubble : bubbles) {
			bubble->draw();
		}
		SDL_RenderPresent(renderer);
	}

	/// Polls and handles all SDL events
	void handleEvents() {
		SDL_PollEvent(&events);
		///User requests quit
		if (events.type == SDL_QUIT)
		{
			running = false;
		}
	}

	/// Deletes invalidated game objects
	void cleanObjects() {
		for (std::vector<GameObject*>::iterator bubble = bubbles.begin(); bubble != bubbles.end();) {
			if (!(*bubble)->isValid()) {
				
				if (bubble == bubbles.begin()) {
					bubbles.erase(bubble);
					bubble = bubbles.begin();
				}
				else {
					std::vector<GameObject*>::iterator thisbubble = bubble;
					delete *thisbubble;
					--bubble;
					bubbles.erase(thisbubble);
				}
			}
			else {
				bubble++;
			}
		}
	}



	void inline generateRandomBubble() {
		bubbles.push_back(addBubble(32, rand() % playZone.w, rand() % (playZone.h / 2) + 100,
			((rand() % 100) *0.005f + 1.5f), 0, (rand() % 100) *0.001f +0.01f, rand() % 5, bubbleTextures[rand() % bubbleTextures.size()]));
	}

	GameObject * addBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, TextureLoader * texture) {
		std::size_t i = bubbles.size();
		GameObject *bubble = new GameObject();
		bubble->addComponent<MovementHandler>((float) posX, (float) posY, velocityX, velocityY, 0.0f, acceleration);
		bubble->addComponent<TileHandler>(renderer, texture, (float) radius * 2 / texture->getRect().h);
		bubble->addComponent<CollisionHandler>(&playZone, true);
		bubble->init();
		bubble->pops = pops;
		
		return bubble;
	}
};