#pragma once
#include "GameObject.h"
#include "TextureLoader.h"
#include "MovementHandler.h"

class TileHandler : public GameComponent {

private:
	bool loadedTexture = false;

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

	~TileHandler() {
		if (loadedTexture) delete texture;
	}

	void update() override{
		
	}

	void init() override {
		owner->img_rect = texture->getRect();
		owner->render_rect = texture->getRect();
		owner->render_rect.h = (int)(owner->render_rect.h * scale);
		owner->render_rect.w = (int)(owner->render_rect.w * scale);
	}

	void draw() override {
		SDL_RenderCopyEx(renderer, texture->getTexture(), &(owner->img_rect), &(owner->render_rect), 0, 0, SDL_FLIP_NONE);
	}

	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture->getTexture(), applied.red, applied.green, applied.blue);
	}
};