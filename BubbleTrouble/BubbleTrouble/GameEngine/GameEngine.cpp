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
	paused = false;

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

	player = manager->addObject<PlayerObject>();

	spike = manager->addObject<SpikeObject>();
	//spike = std::make_unique<SpikeObject>();
	explosionImage = manager->addObject<ExplosionImageObject>();

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

	explosionImage->hide();
	spike->hide();

	player->init();
	spike->init();
	explosionImage->init();

	bubbleExplosion = Mix_LoadWAV("assets/explosionsound.wav");

	for (int i = 0; i < 4; i++) {
		bubbleTextures.emplace_back(std::make_unique<TextureLoader>(renderer, "assets/WhiteBall_128x128.png"));
		bubbleTextures[i]->applyColor(colorarray[i]);
	}
	setState(G_Infinite);
}


void GameEngine::allUpdate() {

	manager->update();
	auto bubbles = manager->getObjectType<BubbleObject>();
	for (auto& bubble : bubbles) {
		if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
			//std::cout << "Collides with bubble\n";
		}
	}

	if (spike->isVisible()) {

		for (auto& bubble : bubbles) {
			if (spike->isVisible() && collidesWithCircle((spike->render_rect), (bubble->render_rect))) {

				bubble->getComponent<SoundHandler>()->play();

				explosionImage->show();

				explosionImage->render_rect = bubble->render_rect;
				explosionImage->getComponent<MovementHandler>()->setPosition(explosionImage->render_rect.x, explosionImage->render_rect.y);

				spike->hide();
				bubble->destroy();
				
				std::cout << "Bubble popped\n";
				if (bubble->pops > 0) {
					std::size_t cindex = randInt<std::size_t>(0, bubbleTextures.size() - 1);
					auto temp = addBubble(bubble->getNextBubble(), bubble->render_rect.x, bubble->render_rect.y, 1, bubbleTextures[cindex].get());
					temp->getComponent<MovementHandler>()->velocity.y = -abs(bubble->getComponent<MovementHandler>()->baseVelocity.y)*0.6;
					
					temp = addBubble(bubble->getNextBubble(), bubble->render_rect.x, bubble->render_rect.y, -1, bubbleTextures[cindex].get());
					temp->getComponent<MovementHandler>()->velocity.y = -abs(bubble->getComponent<MovementHandler>()->baseVelocity.y)*0.6;
				}
				break; //Break so that we pop only one bubble.
			}
		}
	}

}


/// Updates the game state, all objects.
void GameEngine::update() {

	if (!paused) {
		// Re-populate the board if all the bubbles are popped.
		switch (currentState) {
		case G_Menu:
			break;
		case G_MenuOptions:
			break;
		case G_LevelSelect:
			break;
		case G_Infinite:

			allUpdate();
			
			if (manager->getObjectType<BubbleObject>().empty()) {
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
}

/// Render each object on the screen.
void GameEngine::render() {
	SDL_RenderClear(renderer);

	if (!paused) {
		manager->draw();
	}
	else {

	}

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
	manager->clean();
}

/// Set the game state
void GameEngine::setState(GameState state) {
	currentState = state;

	switch (currentState) {
	case G_Menu:

		break;
	case G_MenuOptions:
		break;
	case G_LevelSelect:
		break;
	case G_Infinite:
		/*manager->getObjectType<BubbleObject>().clear();*/
		spike->hide();
		for (int i = 0; i < 3; i++) {
			generateRandomBubble();
		}
		player->render_rect.x = playZone.x + playZone.w / 2 - player->render_rect.w / 2;
		player->render_rect.y = playZone.y + playZone.h - player->render_rect.h;
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
	addBubble(
		static_cast<BubbleType>(randInt(0, 0)),
		randInt(0, playZone.w),
		randInt((int)(playZone.h / 3.0),
		(int)(playZone.h / 2.0)),
		randMinusPlus(),
		bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)].get()
		);
}

BubbleObject * GameEngine::addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture) {
	auto bubble = manager->addObject<BubbleObject>(type, posX, posY, direction, texture, renderer, &playZone, bubbleExplosion);
	bubble->init();
	return bubble;
}

