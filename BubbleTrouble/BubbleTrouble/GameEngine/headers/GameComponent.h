#pragma once

class GameObject;

class GameComponent{
public:
	GameObject * owner;
	//enum ComponentIDs { VECTOR, TILE, COLLISION, KEYBOARD, MOUSE, MOVEMENT, SOUND, TEXTURE, SPRITE } componentID;
	virtual ~GameComponent() = default;
	
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
	
};
