#pragma once
#include "GameObject.h"
#include "TextureLoader.h"


/// Takes care of loading and rendering the texture for the objects
class TileHandler : public GameComponent {

private:
	bool loadedTexture = false;

	/// load a texture et the path with TextureLoader class
	void loadTexture(std::string path) {
		texture = new TextureLoader(renderer, path);
		loadedTexture = true;
	}

	float scale = 0.0f;
	TextureLoader * texture;
	SDL_Renderer * renderer;

public:

	TileHandler(SDL_Renderer * renderer, TextureLoader* texture, float scale) {
		this->renderer = renderer;
		this->texture = texture;
		this->scale = scale;
	}

	TileHandler(SDL_Renderer * renderer, std::string path, float scale) {
		this->renderer = renderer;
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

	void update() override { }
	
	/// Draw the texture at the position of the object
	void draw() override {
		SDL_RenderCopyEx(renderer, texture->getTexture(), &(owner->img_rect), &(owner->render_rect), 0, 0, SDL_FLIP_NONE);
	}


	TextureLoader * getTextureLoader() {
		return texture;
	}

	/// Apply a color modification to the loaded texture.
	void applyColor(Color applied) {
		texture->applyColor(applied);
	}
};