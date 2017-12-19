#include <iostream>
#include "GameEngine/GameEngine.h"


int main(int /*argc*/, char ** /*argv*/) {

	std::unique_ptr<GameEngine> Game{ new GameEngine("Bubble Trouble", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,  1024, 600, SDL_WINDOW_HIDDEN) };

	const Uint32 frameDelay = 1000 / 60;
	Uint32 frameStart;
	Uint32 frameTime;

	// Main game loop
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


	return 0;
}