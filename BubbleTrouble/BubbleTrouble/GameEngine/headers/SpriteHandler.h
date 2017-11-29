#pragma once
#include "GameObject.h"
#include "TextureHandler.h"

class SpriteHandler : public GameComponent {
public:
	TextureHandler * texture;
};