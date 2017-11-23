#pragma once
#include "GameObject.h"

class GameComponent{
public:
	GameComponent() {};
	~GameComponent() {};/*
	GameObject* gameObject;*/
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
};
