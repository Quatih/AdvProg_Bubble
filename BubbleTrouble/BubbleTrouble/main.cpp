#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "GameEngine\GameEngine.h"


int main(int /*argc*/, char ** /*argv*/) {

	GameEngine * Game = new GameEngine("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 480, SDL_WINDOW_HIDDEN);

	Game->init();
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		Game->draw();

	}
	///
	//SDL_Delay(30000);
	return 0;
}