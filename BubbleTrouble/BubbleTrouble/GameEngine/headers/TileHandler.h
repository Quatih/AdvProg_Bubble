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

	TextureLoader * texture;
	SDL_Renderer * renderer;

public:

	TileHandler(SDL_Renderer * renderer, TextureLoader* texture) {
		this->renderer = renderer;
		this->texture = texture;
	}

	TileHandler(SDL_Renderer * renderer, std::string path) {
		this->renderer = renderer;
		loadTexture(path);
	}

	~TileHandler() {
		if (loadedTexture) delete texture;
	}
	void update() override{
		
	}

	void init() override {
	}

	void draw() override {
		SDL_RenderCopyEx(renderer, texture->getTexture(), &(owner->default_rect), &(owner->render_rect), NULL, NULL, SDL_FLIP_NONE);
	}

	void applyColor(Color applied) {
		SDL_SetTextureColorMod(texture->getTexture(), applied.red, applied.green, applied.blue);
	}

};