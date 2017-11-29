#pragma once
#include "GameObject.h"
#include "TextureHandler.h"
#include "MovementHandler.h"

	class TileHandler : public GameComponent {
	public:

	SDL_Texture * texture;
	SDL_Renderer * renderer;
	std::string path;


	TileHandler(SDL_Renderer * renderer, std::string path) {
		this->path = path;
		this->renderer = renderer;

	}

	void update() override{

	}

	void init() override {
		loadTexture();
	}

	void draw() override {
		SDL_RenderCopyEx(renderer, texture, &(owner->default_rect), &(owner->render_rect), NULL, NULL, SDL_FLIP_NONE);
	} 

private:
	void loadTexture() {
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());

		if (loadedSurface == NULL)
		{
			std::cout << "not loaded\n";
		}

		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		if (texture == NULL)
		{
			std::cout << ("Failed to load texture image!\n");
		}
	}
};