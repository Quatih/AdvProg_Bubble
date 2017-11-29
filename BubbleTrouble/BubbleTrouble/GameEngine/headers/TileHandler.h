#pragma once
#include "GameObject.h"
#include "TextureHandler.h"
#include "MovementHandler.h"




struct Color {
	Uint8 red;
	Uint8 green;
	Uint8 blue;
};
Color const RED = { 255, 0, 0 };
Color const GREEN = { 0, 255, 0 };
Color const BLUE = { 0, 0, 255 };
Color const WHITE = { 255, 255, 255 };
Color const BLACK = { 0, 0, 0 };

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
	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture, applied.red, applied.green, applied.blue);
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