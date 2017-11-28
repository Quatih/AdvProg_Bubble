#pragma once
#include "GameComponent.h"
#include "TextureHandler.h"

class TileHandler : public GameComponent {
public:
	TileHandler() {
	}
	void init() {
		std::cout << "TileHandler init\n";
	}
	TextureHandler * texture;

};