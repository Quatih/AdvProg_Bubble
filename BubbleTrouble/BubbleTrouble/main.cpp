#include <iostream>
#include "GameEngine/GameEngine.h"
#include <ctime>

int main(int /*argc*/, char ** /*argv*/) {

	
	std::unique_ptr<GameEngine> Game = std::make_unique<GameEngine>("Bubble Trouble", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,  1024, 600, SDL_WINDOW_HIDDEN);
	

	// Main game loop
	while (Game->isRunning()) {
		//Handle events on queue
		Game->handleEvents();
		Game->update();
		Game->cleanObjects();
		Game->render();
	}

	return 0;
}