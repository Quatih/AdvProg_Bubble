#pragma once
#ifdef __linux__ 
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#endif



class MillisTimer {
public:
	Uint32 startMillis = 0;
	Uint32 pausedMillis = 0;

	bool paused = false;

	void start() {
		paused = false;
		startMillis = SDL_GetTicks();
		pausedMillis = 0;
	}

	void pause() {
		if (!paused) {
			paused = true;
			pausedMillis = SDL_GetTicks() - startMillis;
		}
	}

	void unpause() {
		if (paused) {
			paused = false;
			startMillis = SDL_GetTicks() - pausedMillis;
		}
	}

	Uint32 getMillis() {
		if (!paused) {
			return SDL_GetTicks() - startMillis;
		}
		else {
			return pausedMillis;
		}
	}
};