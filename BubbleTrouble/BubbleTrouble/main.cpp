#include "SDL.h"
#include <iostream>
#include "GameEngine\GameEngine.h"




int main(int /*argc*/, char ** /*argv*/) {
	GameEngine * Game = new GameEngine();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window * window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |	SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	Game->init();
	SDL_Delay(10000);

	return 0;
}