#pragma once

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#else

#include "SDL.h"
#include "SDL_image.h"

#endif

/// Keep track of RGB values.
struct Color {
	Uint8 red;
	Uint8 green;
	Uint8 blue;
};



/// Some basic color values
Color const RED = { 255, 0, 0 };
Color const GREEN = { 0, 255, 0 };
Color const BLUE = { 0, 0, 255 };
Color const WHITE = { 255, 255, 255 };
Color const BLACK = { 0, 0, 0 };

/// TextureLoader is in charge of loading a texture from an image and freeing it upon destruction.
class TextureLoader {
private:
	SDL_Renderer * global_renderer;
	SDL_Texture * texture;
	std::string path;
	SDL_Rect img_rect;
public:

	/// Constructor loads the texture at the passed path
	TextureLoader(SDL_Renderer * renderer, std::string path) {
		global_renderer = renderer;
		this->path = path;
		loadTexture();
	}

	/// Free the loaded texture
	~TextureLoader() {
		SDL_DestroyTexture(texture);
	}


	/// Load the surface from an image at path, then create a texture from the surface.
	void loadTexture() {

		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			std::cout << "Image at path " << "\"" << path.c_str() << "\" failed to load\n";
		}

		img_rect.h = loadedSurface->clip_rect.h;
		img_rect.w = loadedSurface->clip_rect.w;
		img_rect.x = 0;
		img_rect.y = 0;

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

	/// Returns the path of the texture;
	const std::string getPath() {
		return path;
	}

	/// Apply a color modification on the already loaded texture
	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture, applied.red, applied.green, applied.blue);
	}

	/// Return an SDL rect of the loaded image's size.
	SDL_Rect & getRect() {
		return img_rect;
	}

};