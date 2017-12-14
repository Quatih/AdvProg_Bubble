#pragma once
#include "GameObject.h"
#include "TextureLoader.h"

enum BubbleType : std::size_t { Bubble0, Bubble1, Bubble2, Bubble3, Bubble4, MaxBubble} ;
struct BubbleProperties {
	int posX = 0;
	int PosY = 0;
	float velocityX = 0;
	float velocityY = 0;
	float accelerationX = 0;
	float accelerationY = 0;
	int pops = 0;
};

std::vector<BubbleProperties> Properties = { {0,0,0.0f,0.0f,0.0f,0.0f,0} };

class BubbleObject : public GameObject {
private:
	
	BubbleProperties properties;
public:

	int pops = 0;
	BubbleType bubbleType;
	BubbleObject(BubbleType type, int posX, int posY, TextureLoader * texture, SDL_Renderer* renderer, SDL_Rect *playzone) : GameObject(Object_Bubble) {
		bubbleType = type;
		addComponent<MovementHandler>((float)posX, (float)posY);
		addComponent<TileHandler>(renderer, texture, texture->getRect().h);
		addComponent<CollisionHandler>(playZone);
		addComponent<SoundHandler>("assets/explosion.wav");
	}

	void init() override {
		switch (bubbleType) {
		case Bubble0:
			break;
		case Bubble1:
			break;
		case Bubble2:
			break;
		case Bubble3:
			break;
		case Bubble4:
			break;
		default:
			break;
		}

		for (auto& comps : components) {
			comps->init();
		}
	}

	setBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, TextureLoader * texture) {
		
	}
	//void update() override {
	//	if (isValid()) {

	//	}
	//}

};

/// Generate a random bubble
void inline GameEngine::generateRandomBubble() {
	std::unique_ptr<BubbleObject> unique{ addBubble(
		randInt(20, 32),
		randInt(0, playZone.w),
		randInt((int)(playZone.h / 3.0f),
		(int)(playZone.h / 2.0f)),
		randFloatPosNeg(1.3f, 1.75f),
		0.0f,
		randFloat(0.04f, 0.06f),
		randInt(1, 3),
		bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)].get()) };
	bubbles.emplace_back(std::move(unique));
}


/// Add a bubble to the bubble vector and initialize.
BubbleObject * GameEngine::addBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, TextureLoader * texture) {
	BubbleObject * bubble = new BubbleObject();
	bubble->addComponent<MovementHandler>((float)posX, (float)posY, velocityX, velocityY, 0.0f, acceleration);
	bubble->addComponent<TileHandler>(renderer, texture, (float)radius * 2 / texture->getRect().h);
	bubble->addComponent<CollisionHandler>(&playZone);
	bubble->addComponent<SoundHandler>("assets/explosion.wav");
	bubble->init();
	bubble->pops = pops;

	return bubble;
}
