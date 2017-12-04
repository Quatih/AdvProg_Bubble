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

	/// Constructor loads the texture at the passed path
	TextureLoader(SDL_Renderer * renderer, std::string path) {
		global_renderer = renderer;
		this->path = path;
		loadTexture();
	}

	/// Destroy the loaded texture
	~TextureLoader() {
		SDL_DestroyTexture(texture);
	}


	/// Load the surface from an image at path, then create a texture from the surface.
	void loadTexture() {

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			std::cout << "Image at path " << "\"" << path.c_str() << "\" failed to load\n";
		}
		texture = SDL_CreateTextureFromSurface(global_renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
		if (texture == NULL) {
			std::cout << "Texture from path " << "\"" << path.c_str() << "\" failed to load\n";
		}
	}

	/// Returns a pointer to the texture
	SDL_Texture * getTexture() {
		return texture;
	}

	std::string getPath() {
		return path;
	}

	/// Apply a color modification on the already loaded texture
	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture, applied.red, applied.green, applied.blue);
	}

};