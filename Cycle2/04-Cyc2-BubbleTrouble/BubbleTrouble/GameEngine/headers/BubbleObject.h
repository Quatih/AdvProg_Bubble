#pragma once
#include "GameObject.h"
#include "TextureLoader.h"
#include "Components.h"


/// Different bubble types
enum BubbleType : std::size_t { Bubble0, Bubble1, Bubble2, Bubble3, Bubble4 };

/// Struct to hold different properties of the bubble types
struct BubbleProperties {
	double radius = 0;
	int pops = 0;
	double velocityX = 0;
	double velocityY = 0;
	double accelerationX = 0;
	double accelerationY = 0;
};

const double Base_BubbleX_velocity = 2.1;
const double Base_BubbleY_velocity = 7.5;
const double Base_BubbleY_acceleration = 0.15;
///Vector with the bubble properties
const std::vector<BubbleProperties> Properties = {
{ 8,	0, Base_BubbleX_velocity, Base_BubbleY_velocity,		0, Base_BubbleY_acceleration },
{ 14,	1, Base_BubbleX_velocity, Base_BubbleY_velocity + 0.5,	0, Base_BubbleY_acceleration },
{ 20,	2, Base_BubbleX_velocity, Base_BubbleY_velocity + 1,	0, Base_BubbleY_acceleration },
{ 28,	3, Base_BubbleX_velocity, Base_BubbleY_velocity + 1.5,	0, Base_BubbleY_acceleration },
{ 36,	4, Base_BubbleX_velocity, Base_BubbleY_velocity + 2,	0, Base_BubbleY_acceleration }
};

///Class for dealing with different bubbles
class BubbleObject : public GameObject {
private:

public:

	int pops = 0;
	BubbleType bubbleType;
	BubbleObject(BubbleType type, int posX, int posY, int direction, TextureLoader * texture, SDL_Renderer* renderer, SDL_Rect *playZone, Mix_Chunk* chunk) : GameObject(Object_Bubble) {
		bubbleType = type;
		addComponent<MovementHandler>((double)posX, (double)posY, Properties[type].velocityX*direction, 0, Properties[type].accelerationX, Properties[type].accelerationY);
		addComponent<TileHandler>(renderer, texture, Properties[type].radius * 2 / texture->getRect().h);
		addComponent<CollisionHandler>(playZone);
 		addComponent<SoundHandler>(chunk);
		pops = Properties[type].pops;
		getComponent<MovementHandler>()->baseVelocity.y = Properties[type].velocityY;
	}

	/// Get the next bubble type in the line
	BubbleType getNextBubble() const {
		switch (bubbleType)	{
		case Bubble0:
			return Bubble0;
			break;
		case Bubble1:
			return Bubble0;
			break;
		case Bubble2:
			return Bubble1;
			break;
		case Bubble3:
			return Bubble2;
			break;
		case Bubble4:
			return Bubble3;
			break;
		default:
			return Bubble0;
			break;
		}
	}
};