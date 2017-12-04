#pragma once
#include "GameObject.h"
#include "SDL_image.h"

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


class TextureLoader {
private:
	SDL_Renderer * global_renderer;
	SDL_Texture * texture;
	std::string path;
public:

	TextureLoader(SDL_Renderer * renderer, std::string path) {
		global_renderer = renderer;
		this->path = path;
		loadTexture();
	}

	~TextureLoader() {
		SDL_DestroyTexture(texture);
	}

	void loadTexture() {
		//Load image at specified path

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());

		if (loadedSurface == NULL)
		{
			std::cout << "not loaded\n";
		}

		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(global_renderer, loadedSurface);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		if (texture == NULL)
		{
			std::cout << ("Failed to load texture image!\n");
		}
	}

	SDL_Texture * getTexture() {
		return texture;
	}

	std::string getPath() {
		return path;
	}

	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture, applied.red, applied.green, applied.blue);
	}

};