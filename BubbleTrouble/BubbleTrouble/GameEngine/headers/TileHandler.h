#pragma once
#include "GameObject.h"
#include "TextureLoader.h"

extern SDL_Renderer * renderer;
/// Takes care of loading and rendering the texture for the objects
class TileHandler : public GameComponent {
private:
	bool loadedTexture = false;

	/// load a texture et the path with TextureLoader class
	void loadTexture(std::string path) {
		texture = new TextureLoader(path);
		loadedTexture = true;
	}
	TextureLoader * texture;


public:

	double scale = 1;

	TileHandler(TextureLoader* texture, double scale) {
		this->texture = texture;
		this->scale = scale;
	}

	TileHandler(std::string path, double scale) {
		loadTexture(path);
		this->scale = scale;
	}

	/// If this component created a texture itself, deallocate it from memory.
	~TileHandler() {
		if (loadedTexture) delete texture;
	}

	/// Sets the size of the objects rectangles depending on the size of the loaded image
	void init() override {
		owner->img_rect = texture->getRect();
		owner->render_rect = texture->getRect();
		owner->render_rect.h = (int)(owner->render_rect.h * scale);
		owner->render_rect.w = (int)(owner->render_rect.w * scale);

		if (owner->hasComponent<MovementHandler>()) {
			owner->render_rect.x = (int)owner->getComponent<MovementHandler>()->position.x;
			owner->render_rect.y = (int)owner->getComponent<MovementHandler>()->position.y;
		}
		else {
			owner->render_rect.x = 0;
			owner->render_rect.y = 0;
		}
	}

	/// Draw the texture at the position of the object
	void draw() override {
		SDL_RenderCopyEx(renderer, texture->getTexture(), &(owner->img_rect), &(owner->render_rect), 0, 0, SDL_FLIP_NONE);
	}

	/// return a pointer to the stored textureLoader
	TextureLoader * getTextureLoader() const {
		return texture;
	}

	/// Change the texture to the loaded texture.
	void setTextureLoader(TextureLoader * textureLoader) {
		if (loadedTexture) delete texture;
		loadedTexture = false;
		texture = textureLoader;
		init();
	}

	/// Load a new texture from the path.
	TextureLoader * loadNewTexture(std::string path) {
		if (loadedTexture) delete texture;

		loadTexture(path);

		init();
		return texture;
	}

	/// Apply a color modification to the loaded texture.
	void applyColor(Color applied) {
		texture->applyColor(applied);
	}

	/// Apply a color and alpha modification to the loaded texture.
	void applyColor(SDL_Color applied) {
		texture->applyColor(applied);
	}

};