#pragma once
#include "GameObject.h"
#include "TextureLoader.h"

class SpriteHandler : public GameComponent {
public:
	TextureLoader * texture;
};