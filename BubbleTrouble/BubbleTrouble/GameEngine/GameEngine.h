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

public:

	SDL_Renderer * renderer;

	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag){
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

	~GameEngine(){
		delete player;
		bubbles.clear();
		running = false;
	};

	bool isRunning() {
		return running;
	}

	void init() {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_ShowWindow(window);

		int width, height;
		SDL_GetWindowSize(window, &width, &height);

		player = new GameObject(32, 32, 1.5); //height-player->render_rect.h
		player->addComponent<MovementHandler>(1.0f, 1.0f);
		player->getComponent<MovementHandler>()->setVelocity(0.0f, 0.0f);

		player->addComponent<TileHandler>(renderer, "assets/square_B.png");
		player->addComponent<KeyboardHandler>(&events, 2.0f, true);

		player->addComponent<CollisionHandler>(&playZone, false);


		addBubble(32, 100, height / 2, 2.5f, 0.2f, RED);
		addBubble(48, 300, height / 3, 2.5f, 0.2f, GREEN);
		addBubble(16, 600, height / 4, 2.5f, 0.2f, BLUE);
	}
	void update() {
		player->update();
		for (auto bubble : bubbles) {
			bubble->update();
		}
		for (auto bubble : bubbles) {
			if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
				std::cout << "Collides with bubble\n";
				bubble->destroy();
			}			
		}

	}

	void render() {
		SDL_RenderClear(renderer);
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
				std::vector<GameObject*>::iterator thisbubble = bubble;
				if (bubble == bubbles.begin()) {
					bubbles.erase(thisbubble);
					bubble = bubbles.begin();
				}
				else {
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

	void addBubble(int radius, int posX, int posY, float velocity, float acceleration, Color color) {
		std::size_t i = bubbles.size();
		bubbles.push_back(new GameObject(512, 512, (float)radius*2/512));

		bubbles[i]->addComponent<MovementHandler>((float) posX, (float) posY);
		bubbles[i]->addComponent<TileHandler>(renderer, "assets/Ball.png");
		bubbles[i]->getComponent<TileHandler>()->applyColor(color);
		bubbles[i]->addComponent<CollisionHandler>(&playZone, true);
		bubbles[i]->getComponent<MovementHandler>()->setVelocity(velocity, 0);
		bubbles[i]->getComponent<MovementHandler>()->acceleration.x = 0.0;
		bubbles[i]->getComponent<MovementHandler>()->acceleration.y = acceleration;
	}
};