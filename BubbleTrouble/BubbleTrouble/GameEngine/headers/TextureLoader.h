#pragma once

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#else

#include <SDL.h>
#include <SDL_image.h>

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

extern SDL_Renderer * renderer;

/// TextureLoader is in charge of loading a texture from an image and freeing it upon destruction.
class TextureLoader {
private:
	SDL_Texture * texture;
	std::string path;
	SDL_Rect img_rect;
public:

	/// Constructor loads the texture at the passed path
	TextureLoader(std::string path) {
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

		img_rect = loadedSurface->clip_rect;
		texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		SDL_FreeSurface(loadedSurface);
		if (texture == NULL) {
			std::cout << "Texture from path " << "\"" << path.c_str() << "\" failed to load\n";
		}
	}

	/// Returns a pointer to the texture
	SDL_Texture * getTexture() const {
		return texture;
	}

	/// Returns the path of the texture;
	const std::string getPath() const{
		return path;
	}

	/// Apply a color modification on the already loaded texture
	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture, applied.red, applied.green, applied.blue);
	}

	/// Apply a color and alpha modification on the already loaded texture
	void applyColor(SDL_Color applied) {
		SDL_SetTextureColorMod(texture, applied.r, applied.g, applied.b);
		SDL_SetTextureAlphaMod(texture, applied.a);
	}

	/// Return an SDL rect of the loaded image's size.
	const SDL_Rect & getRect() const {
		return img_rect;
	}
};