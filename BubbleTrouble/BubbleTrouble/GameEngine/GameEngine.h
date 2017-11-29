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


public:

	SDL_Renderer * renderer;

	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag){
		SDL_Init(SDL_INIT_EVERYTHING);
		window = SDL_CreateWindow(title.c_str(), winposx, winposy, winwidth, winheight, flag);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
		std::cout << "Game Engine Constructed.\n";
		running = true;
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
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_ShowWindow(window);

		int width, height;
		SDL_GetWindowSize(window, &width, &height);

		player = new GameObject(32, 32, 1.5); //height-player->render_rect.h
		player->addComponent<MovementHandler>(1.0f, 1.0f);
		player->getComponent<MovementHandler>()->setVelocity(0.0f, 0.0f);

		player->addComponent<TileHandler>(renderer, "assets/square_B.png");
		player->addComponent<KeyboardHandler>(&events, 2.0f, true);

		player->addComponent<CollisionHandler>(0, 0, width, height);

		bubbles.push_back(new GameObject(32, 32, 1));
		bubbles[0]->addComponent<MovementHandler>(100.0f, 0.0f);
		bubbles[0]->addComponent<TileHandler>(renderer, "assets/white_ball.png");
		bubbles[0]->addComponent<CollisionHandler>(0, 0, width, height);
		bubbles[0]->getComponent<MovementHandler>()->setVelocity(2, 0);
		bubbles[0]->getComponent<MovementHandler>()->acceleration.x = 0.0;
		bubbles[0]->getComponent<MovementHandler>()->acceleration.y = 0.1;
		bubbles.push_back(new GameObject(32, 32, 1));
		bubbles[1]->addComponent<MovementHandler>(1000.0f, 400.0f);
		bubbles[1]->addComponent<TileHandler>(renderer, "assets/white_ball.png");
		bubbles[1]->addComponent<CollisionHandler>(0, 0, width, height);
		bubbles[1]->getComponent<MovementHandler>()->setVelocity(6, 0);
		bubbles[1]->getComponent<MovementHandler>()->acceleration.x = 0.0;
		bubbles[1]->getComponent<MovementHandler>()->acceleration.y = 0.2;
	}
	void update() {


		player->update();
		for (auto bubble : bubbles) {
			bubble->update();
		}
		//for (auto bubble : bubbles) {
		//	if (collidesWithCirlce((player->render_rect), (bubble->render_rect))) {
		//		std::cout << "Collides with bubble\n";
		//		bubble->destroy();
		//	}			
		//}

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
};