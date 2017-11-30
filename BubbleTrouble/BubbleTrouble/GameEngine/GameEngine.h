#pragma once
#include "headers\GameObject.h"
#include "headers\Components.h"
#include "headers\Scoreboard.h"
#include "headers\SoundPlayer.h"
#include "headers\KeyboardHandler.h"
#include <vector>
#include "headers\Menu.h"
#include "headers\Map.h"
#include "SDL.h"
#include "SDL_image.h"
#include <random>

class GameEngine {
private:
	GameObject * player;
	std::vector<GameObject*> bubbles;
	GameObject * spike;
	Map * map;
	SDL_Window * window;
	SDL_Event events;
	bool running;
	SDL_Rect playZone; 
	SDL_Rect spikeZone;

	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };
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
	};

	~GameEngine() {
		delete player;
		bubbles.clear();
		running = false;
		delete spike;
	};

	bool isRunning() {
		return running;
	}

	void init() {
		SDL_SetRenderDrawColor(renderer, 200, 255, 255, 255);
		SDL_ShowWindow(window);

		int width, height;
		SDL_GetWindowSize(window, &width, &height);

		player = new GameObject(9, 28, 4); //height-player->render_rect.h
		spike = new GameObject(15, 1600, 1);
		player->addComponent<MovementHandler>((float)playZone.w/2, (float)playZone.h, 0.0f, 0.0f, 0.0f, 0.0f);
		player->getComponent<MovementHandler>()->setVelocity(0.0f, 0.0f);

		player->addComponent<TileHandler>(renderer, "assets/weirdguy2.png");
		player->addComponent<KeyboardHandler>(&events, 3.5f, false, spike);
		player->addComponent<CollisionHandler>(&playZone, false);
		
		spike->addComponent<MovementHandler>(0.0f, 0.0f, 0.0f, -4.5f, 0.0f, 0.0f);
		spike->addComponent<TileHandler>(renderer, "assets/spike3.png");

		spikeZone = playZone;
		spikeZone.h = 2000;
		spikeZone.w = 2000;
		spike->addComponent<CollisionHandler>(&spikeZone, false);
		spike->destroy();
		//spike->destroy();

		for (int i = 0; i < 1; i++) {
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
			if (spike->render_rect.y <= 5) {
				spike->destroy();
			}
			for (auto bubble : bubbles) {
				if (collidesWithRect((spike->render_rect), (bubble->render_rect))) {
					spike->destroy();
					bubble->destroy();

					std::cout << "Bubble popped\n";
					if (bubble->pops > 0) {
						int cindex = rand() % 4;
						tempbubbles.push_back(addBubble(bubble->render_rect.h / 4, bubble->render_rect.x, bubble->render_rect.y, bubble->getComponent<MovementHandler>()->velocity.x,
							-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.8), bubble->getComponent<MovementHandler>()->acceleration.y, bubble->pops - 1, colorarray[cindex]));
						tempbubbles.push_back(addBubble(bubble->render_rect.h / 4, bubble->render_rect.x, bubble->render_rect.y, -bubble->getComponent<MovementHandler>()->velocity.x,
							-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.8), bubble->getComponent<MovementHandler>()->acceleration.y, bubble->pops - 1, colorarray[cindex]));
					}

					
				}
			}
			for (auto bubble : tempbubbles) {
				bubbles.push_back(bubble);
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

	void quit() {
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

	void inline generateRandomBubble() {
		bubbles.push_back(addBubble(rand() % 50 + 10, rand() % playZone.w, rand() % (playZone.h / 2 - 100) + 100,
			((rand() % 100) *0.01f + 2.0f), 0, (rand() % 100) *0.001f +0.01f, rand() % 5, colorarray[rand() % 4]));
	}

	GameObject * addBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, Color color) {
		std::size_t i = bubbles.size();
		GameObject *bubble = new GameObject(512, 512, (float)radius*2/512);

		bubble->addComponent<MovementHandler>((float) posX, (float) posY);
		bubble->addComponent<TileHandler>(renderer, "assets/white_ball.png");
		bubble->getComponent<TileHandler>()->applyColor(color);
		bubble->addComponent<CollisionHandler>(&playZone, true);
		bubble->getComponent<MovementHandler>()->setVelocity(velocityX, velocityY);
		bubble->getComponent<MovementHandler>()->acceleration.x = 0.0;
		bubble->getComponent<MovementHandler>()->acceleration.y = acceleration;
		bubble->pops = pops;
		return bubble;
	}
};