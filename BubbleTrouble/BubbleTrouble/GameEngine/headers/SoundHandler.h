#pragma once
#include "GameObject.h"
#include "MovementHandler.h"
#include <cmath>

/// Handles sound for the object
class SoundHandler : public GameComponent {
public:
	//Mix_Music* test = NULL;
	Mix_Chunk* audio;
	bool loaded = false;
	std::string paths;

	/// Load a chunk from a path.
	SoundHandler(std::string path) {
		this->paths = path.c_str();
		loaded = true;
	}

	/// Take a pointer to the chunk, to reduce memory usage.
	SoundHandler(Mix_Chunk* chunk) {
		audio = chunk;
	}

	/// Free the chunk if this handler loaded one
	~SoundHandler() {
		if (loaded) Mix_FreeChunk(audio);
	}

	void init() override {
		
		if (loaded) {	
			//audio = Mix_LoadMUS(paths.c_str());
			audio = Mix_LoadWAV(paths.c_str());

			if (audio == NULL)
				std::cout << "sound init failed\n";
			else
				std::cout << "Sound init\n";
		}
	}

	void update() override {

	}

	void play() {
		if (Mix_PlayChannel(2, audio, 0) == -1) {
			std::cout << "Error playing sound\n";
		}
	}


};