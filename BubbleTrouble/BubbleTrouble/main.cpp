#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "GameEngine\GameEngine.h"


int main(int /*argc*/, char ** /*argv*/) {

	GameEngine * Game = new GameEngine("Bubble Trouble", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,  1280, 720, SDL_WINDOW_HIDDEN);

	Game->init();
	bool quit = false;

	//Event handler
	
	const int frameDelay = 1000 / 60;
	Uint32 frameStart;
	int frameTime;
	//While application is running
	while (Game->isRunning())
	{
		frameStart = SDL_GetTicks();
		//Handle events on queue

		Game->handleEvents();
		Game->update();
		Game->cleanObjects();
		Game->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime){
			SDL_Delay(frameDelay - frameTime);
		}

	}

	Game->quit();
	return 0;
}