#pragma once
#include "GameComponent.h"
#include "SDL_image.h"


class TextureHandler : public GameComponent {
private:
	SDL_Renderer * global_renderer;
	SDL_Texture * texture;
public:
	

	TextureHandler(SDL_Renderer * render) {
		global_renderer = render;
	}


	void loadTexture(const char * path) {
		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path);

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
			std::cout <<("Failed to load texture image!\n");
		}
		
	}
	SDL_Texture * getTexture() {
		return texture;
	}



};