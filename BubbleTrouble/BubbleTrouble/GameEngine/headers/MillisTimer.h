#pragma once
#ifdef __linux__ 
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif


/// Class for keeping track of millisecond timing with pause functionality
class MillisTimer {
public:
	Uint32 startMillis = 0;
	Uint32 pausedMillis = 0;

	bool paused = true;

	/// Start the timer
	void start() {
		paused = false;
		startMillis = SDL_GetTicks();
		pausedMillis = 0;
	}
	
	/// Pause the timer, keep track of the elapsed milliseconds for unpausing
	void pause() {
		if (!paused) {
			paused = true;
			pausedMillis = SDL_GetTicks() - startMillis;
		}
	}
	
	/// unpause, adjust startMillis appropriately
	void unpause() {
		if (paused) {
			paused = false;
			startMillis = SDL_GetTicks() - pausedMillis;
		}
	}

	/// return the appropriate millis
	Uint32 getMillis() {
		if (!paused) {
			return SDL_GetTicks() - startMillis;
		}
		else {
			return pausedMillis;
		}
	}

	/// Reset and stop the timer.
	void reset() {
		paused = true;
		startMillis = 0;
		pausedMillis = 0;
	}
};