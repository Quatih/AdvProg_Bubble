#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include <cmath>

/// Handles all keyboard input for the player
class SoundHandler : public GameComponent {
public:
	//Mix_Music* test = NULL;
	Mix_Chunk* test = NULL;
	
	std::string paths;

	SoundHandler(std::string path) {
		this->paths = path.c_str();
	}

	void init() override {
		

		/*test = Mix_LoadMUS(paths.c_str());*/
		test = Mix_LoadWAV(paths.c_str());
		
		if (test == NULL)
			std::cout << "sound init failed\n";
		else
			std::cout << "Sound init\n";

	}

	void update() override {
	}

	/*~SoundHandler() {
		Mix_FreeMusic(test);
		SDL_CloseAudio();
	}*/
};