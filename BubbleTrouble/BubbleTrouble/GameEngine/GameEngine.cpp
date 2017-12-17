#include "GameEngine.h"
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/RandomInterface.h"
#include "headers/SoundHandler.h"
#include "headers/CollisionChecks.h"

/// Constructor creates the window and renderer
GameEngine::GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag) {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title.c_str(), winposx, winposy, winwidth, winheight, flag);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	std::cout << "Game Engine Constructed.\n";
	running = true;
	playZone.x = 0;
	playZone.y = 0;
	playZone.h = winheight;
	playZone.w = winwidth;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << Mix_GetError();
	}

	setState(G_Init);
	// Set render quality to 1, so that scaled objects are dithered a little
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

/// Free all allocated memory, hopefully.
GameEngine::~GameEngine() {
	bubbles.clear();
	running = false;

	bubbleTextures.clear();
	Mix_CloseAudio();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

/// Initialize player, spike and bubbles.

void GameEngine::init() {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_ShowWindow(window);

	manager = std::make_unique<ObjectManager>();

	player = std::make_unique<PlayerObject>();

	spike = manager->addObject<GameObject>(Object_Spike);

	//spike = std::make_unique<SpikeObject>();
	explosionImage = std::make_unique<ExplosionImageObject>();

	player->addComponent<KeyboardHandler>(3.8, false, spike);
	player->addComponent<MovementHandler>((double)playZone.w / 2, (double)playZone.h);
	player->addComponent<TileHandler>(renderer, "assets/duder4.png", 0.9);
	player->addComponent<CollisionHandler>(&playZone);

	spike->addComponent<MovementHandler>(0.0, 0.0, 0.0, -4.8, 0.0, 0.0);
	spike->addComponent<TileHandler>(renderer, "assets/spike4.png", 1.0);
	spike->addComponent<CollisionHandler>(&playZone);
	spike->addComponent<SoundHandler>("assets/spikesound2.wav");

	explosionImage->addComponent <TileHandler>(renderer, "assets/collision.png", 0.5);
	explosionImage->addComponent<MovementHandler>(0.0, 0.0);

	explosionImage->destroy();
	spike->destroy();

	player->init();
	spike->init();
	explosionImage->init();

	

	for (int i = 0; i < 4; i++) {
		bubbleTextures.emplace_back(std::make_unique<TextureLoader>(renderer, "assets/WhiteBall_128x128.png"));
		bubbleTextures[i]->applyColor(colorarray[i]);
	}
	setState(G_Infinite);
}

/// Updates the game state, all objects.

void GameEngine::update() {
	
	player->update();


	for (auto& bubble : bubbles) {
		bubble->update();
	}
	
	for (auto& bubble : bubbles) {
		if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
			//std::cout << "Collides with bubble\n";
		}
	}
	
	if (spike->isValid()) {
		spike->update();

		std::vector<BubbleObject*> tempbubbles;
		for (auto& bubble : bubbles) {
			if (collidesWithCircle((spike->render_rect), (bubble->render_rect))) {

				Mix_HaltMusic();
				if (Mix_PlayMusic(bubble->getComponent<SoundHandler>()->test, 1) != -1) {
					std::cout << "Sound playing for explosion\n";
				}
				else
					std::cout << "sound playing inside the collision loop failing\n";

				explosionImage->setValid();

				explosionImage->render_rect = bubble->render_rect;
				explosionImage->getComponent<MovementHandler>()->setPosition(explosionImage->render_rect.x, explosionImage->render_rect.y);

				spike->destroy();
				bubble->destroy();
				
				std::cout << "Bubble popped\n";
				if (bubble->pops > 0) {
					std::size_t cindex = randInt<std::size_t>(0, bubbleTextures.size() - 1);
					tempbubbles.emplace_back(addBubble(bubble->getNextBubble(), bubble->render_rect.x, bubble->render_rect.y, 1, bubbleTextures[cindex].get()));
					tempbubbles[0]->getComponent<MovementHandler>()->velocity.y = -abs(bubble->getComponent<MovementHandler>()->baseVelocity.y)*0.6;

					tempbubbles.emplace_back(addBubble(bubble->getNextBubble(), bubble->render_rect.x, bubble->render_rect.y, -1, bubbleTextures[cindex].get()));
					tempbubbles[1]->getComponent<MovementHandler>()->velocity.y = -abs(bubble->getComponent<MovementHandler>()->baseVelocity.y)*0.6;
				}
				break; //Break so that we pop only one bubble.
			}
		}

		// add the bubbles in later so that they're not iterated over in the previous loop.
		for (auto& bubble : tempbubbles) {
			std::unique_ptr<BubbleObject> unique { bubble };
			bubbles.emplace_back(std::move(unique));
		}
	}

	explosionImage->update();

	// Re-populate the board if all the bubbles are popped.
	switch (currentState) {
	case G_Init:
		init();
		break;
	case G_Menu:
		break;
	case G_MenuOptions:
		break;
	case G_LevelSelect:
		break;
	case G_Infinite:

		if (bubbles.empty()) {
			for (int i = 0; i < 3; i++) {
				generateRandomBubble();
			}
		}

		break;
	case G_Level1:
		break;
	case G_Level2:
		break;
	case G_Level3:
		break;
	case G_Level4:
		break;
	case G_Level5:
		break;
	case G_Level6:
		break;
	case G_Level7:
		break;
	case G_Level8:
		break;
	case G_Level9:
		break;
	case G_Level10:
		break;
	default:
		break;
	}

}

/// Render each object on the screen.
void GameEngine::render() {
	SDL_RenderClear(renderer);
	spike->draw();
	player->draw();


	for (auto& bubble : bubbles) {
		bubble->draw();
	}
	explosionImage->draw();
	SDL_RenderPresent(renderer);
}

/// Polls and handles all SDL events
void GameEngine::handleEvents() {
	SDL_PollEvent(&events);
	// User requests quit
	if (events.type == SDL_QUIT) {
		running = false;
	}
}

/// Deletes invalidated game objects
void GameEngine::cleanObjects() {
	for (auto bubble = bubbles.begin(); bubble != bubbles.end();) {
		if (!(*bubble)->isValid()) {
			bubble = bubbles.erase(bubble);
		}
		else {
			bubble++;
		}
	}
}

/// Set the game state
void GameEngine::setState(GameState state) {
	currentState = state;
	switch (currentState) {
	case G_Init:
		init();
		break;
	case G_Menu:
		break;
	case G_MenuOptions:
		break;
	case G_LevelSelect:
		break;
	case G_Infinite:
		break;
	case G_Level1:
		break;
	case G_Level2:
		break;
	case G_Level3:
		break;
	case G_Level4:
		break;
	case G_Level5:
		break;
	case G_Level6:
		break;
	case G_Level7:
		break;
	case G_Level8:
		break;
	case G_Level9:
		break;
	case G_Level10:
		break;
	default:
		break;
	}
}

/// Generate a random bubble
void inline GameEngine::generateRandomBubble() {

	std::unique_ptr<BubbleObject> unique{
		addBubble(
		static_cast<BubbleType>(randInt(1, 4)),
		randInt(0, playZone.w),
		randInt((int)(playZone.h / 3.0),
		(int)(playZone.h / 2.0)),
		randMinusPlus(),
		bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)].get()) };
	bubbles.emplace_back(std::move(unique));
}


/// Add a bubble to the bubble vector and initialize.
//BubbleObject * GameEngine::addBubble(int radius, int posX, int posY, double velocityX, double velocityY, double acceleration, int pops, TextureLoader * texture) {
BubbleObject * GameEngine::addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture) {
	BubbleObject * bubble = new BubbleObject(type, posX, posY, direction, texture, renderer, &playZone);
	bubble->init();
	return bubble;
}
