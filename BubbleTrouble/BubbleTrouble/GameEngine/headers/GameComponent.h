#pragma once
#include "GameObject.h"
#include "Components.h"

enum ComponentIds { VECTOR, TILE, COLLISION, KEYBOARD, MOUSE, MOVEMENT, SOUND, TEXTURE, SPRITE };

class GameComponent{
public:
	GameObject * owner;
	ComponentIds ID;

	virtual ~GameComponent() {};
	
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	
};
