#include "SDL.h"
#include <iostream>
#include "GameEngine\GameEngine.h"




int main(int /*argc*/, char ** /*argv*/) {

	GameEngine * Game = new GameEngine("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 480, SDL_WINDOW_HIDDEN);
	SDL_SetRenderDrawColor(Game->renderer, 255, 255, 255, 255);
	SDL_RenderClear(Game->renderer);
	SDL_RenderPresent(Game->renderer);
	Game->init();

	bool quit = false;

	//Event handler
	SDL_Event e_xOut;

	KeyboardHandler *key = new KeyboardHandler();
	key->init();
	key->update();
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e_xOut) != 0)
		{
			//User requests quit
			if (e_xOut.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e_xOut.type == SDL_KEYDOWN)
			{
				key->update();
			}
			//std::cout << "I am always here\n";
			
			
			
			
		}

	}
	///
	//SDL_Delay(30000);
	return 0;
}