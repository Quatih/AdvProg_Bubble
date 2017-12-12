#pragma once
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/CollisionChecks.h"
#include "headers/RandomInterface.h"
#include <vector>
#include <random>
#include <ctime>

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#else

#include "SDL.h"
#include "SDL_image.h"

#endif

/// Handles all the game logic
class GameEngine {
private:

	GameObject * player;
	std::vector<GameObject*> bubbles;
	GameObject * spike;

	SDL_Window * window;
	SDL_Event events;
	bool running;

	/// Rectangle for the playingZone objects can be within
	SDL_Rect playZone;
	/// The zone the Spike can be within
	SDL_Rect spikeZone;

	/// Used for random access of color.
	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };

	/// Use this to re-use the bubble textures and minimize memory allocation.
	std::vector<TextureLoader*> bubbleTextures;

public:
	SDL_Renderer * renderer;

	/// Constructor creates the window and renderer
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

		// Set render quality to 1, so that scaled objects are dithered a little
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	};

	/// Free all allocated memory, hopefully.
	~GameEngine() {
		delete player;
		delete spike;
		bubbles.clear();
		running = false;

		bubbleTextures.clear();
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};

	bool inline isRunning() const {
		return running;
	} 


	/// Initialize player, spike and bubbles.
	void init() {

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_ShowWindow(window);

		player = new GameObject(Object_Player);
		spike = new GameObject(Object_Spike);

		player->addComponent<KeyboardHandler>(3.5f, false, spike);
		player->addComponent<MovementHandler>((float)playZone.w / 2, (float)playZone.h);
		player->addComponent<TileHandler>(renderer, "assets/duder3.png", 1.0f);
		player->addComponent<CollisionHandler>(&playZone);


		spike->addComponent<MovementHandler>(0.0f, 0.0f, 0.0f, -4.8f, 0.0f, 0.0f);
		spike->addComponent<TileHandler>(renderer, "assets/spike4.png", 1.0f);
		spike->addComponent<CollisionHandler>(&playZone);
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

	/// Updates the game state, all objects.
	void update() {

		player->update();
		for (auto bubble : bubbles) {
			bubble->update();
		}


		for (auto bubble : bubbles) {
			if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
				//std::cout << "Collides with bubble\n";
			}
		}
		if (spike->isValid()) {
			spike->update();

			//If the spike has reached the top, destroy it.
			if (spike->render_rect.y < playZone.y) {
				spike->destroy();
			}

			std::vector<GameObject*> tempbubbles;
			for (auto bubble : bubbles) {
				if (collidesWithCircle((spike->render_rect), (bubble->render_rect))) {

					spike->destroy();
					bubble->destroy();
					std::cout << "Bubble popped\n";
					if (bubble->pops > 0) {
						std::size_t cindex = randInt<std::size_t>(0, bubbleTextures.size() - 1);
						tempbubbles.push_back(
							addBubble(bubble->render_rect.h / 3, 
							bubble->render_rect.x, bubble->render_rect.y, 
							bubble->getComponent<MovementHandler>()->velocity.x,
							-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.65f), 
							bubble->getComponent<MovementHandler>()->acceleration.y, 
							bubble->pops - 1, 
							bubbleTextures[cindex])
						);
						tempbubbles.push_back(
							addBubble(bubble->render_rect.h / 3, 
							bubble->render_rect.x, bubble->render_rect.y, 
							-bubble->getComponent<MovementHandler>()->velocity.x,
							-abs(bubble->getComponent<MovementHandler>()->velocity.y*0.65f), 
							bubble->getComponent<MovementHandler>()->acceleration.y, 
							bubble->pops - 1, 
							bubbleTextures[cindex])
						);
					}
					break; //Break so that we pop only one bubble.
				}
			}

			// add the bubbles in later so that they're not iterated over in the previous loop.
			for (auto bubble : tempbubbles) {
				bubbles.push_back(bubble);
			}
		}

		// Re-populate the board if all the bubbles are popped.
		if (bubbles.empty()) {
			for (int i = 0; i < 4; i++) {
				generateRandomBubble();
			}
		}
	}

	/// Render each object on the screen.
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
		// User requests quit
		if (events.type == SDL_QUIT) {
			running = false;
		}
	}

	/// Deletes invalidated game objects
	void cleanObjects() {
		for (std::vector<GameObject*>::iterator bubble = bubbles.begin(); bubble != bubbles.end();) {
			if (!(*bubble)->isValid()) {
				bubble = bubbles.erase(bubble);
			}
			else {
				bubble++;
			}
		}
	}


	/// Generate a random bubble
	void inline generateRandomBubble() {
		bubbles.push_back(addBubble(randInt(20, 32), 
			randInt(0, playZone.w), 
			randInt((int)(playZone.h/3.0f), 
			(int)(playZone.h/2.0f)),
			randFloatPosNeg(1.3f, 1.75f),
			0.0f, 
			randFloat(0.04f, 0.06f), 
			randInt(1, 3), 
			bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)])
		);
	}

	/// Add a bubble to the bubble vector and initialize.
	GameObject * addBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, TextureLoader * texture) {
		GameObject *bubble = new GameObject(Object_Bubble);
		bubble->addComponent<MovementHandler>((float) posX, (float) posY, velocityX, velocityY, 0.0f, acceleration);
		bubble->addComponent<TileHandler>(renderer, texture, (float) radius * 2 / texture->getRect().h);
		bubble->addComponent<CollisionHandler>(&playZone);
		bubble->init();
		bubble->pops = pops;
		
		return bubble;
	}
};