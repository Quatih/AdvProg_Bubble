#include "GameEngine.h"
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/RandomInterface.h"
#include "headers/SoundHandler.h"
#include "headers/CollisionChecks.h"
#include <string>
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

	init();
	setState(G_Infinite);

	// Set render quality to 1, so that scaled objects are dithered a little
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

/// Free all allocated memory, hopefully.
GameEngine::~GameEngine() {
	refresh();
	TTF_CloseFont(font);
	TTF_Quit();
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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << Mix_GetError();
	}
	TTF_Init();
	font = TTF_OpenFont("assets/FreeSansBold.ttf", 24);
	manager = std::make_unique<ObjectManager>();
	for (int i = 0; i < 4; i++) {
		TextureLoader * bubb = new TextureLoader(renderer, "assets/WhiteBall_128x128.png");
		bubbleTextures.emplace_back(std::move(bubb));
		bubb->applyColor(colorarray[i]);
	}

	//std::unique_ptr<TextureLoader> Textl (new TextureLoader();
	heartTexture = std::make_unique<TextureLoader>(renderer, "assets/heart.png");

	bubbleExplosion = Mix_LoadWAV("assets/pop.wav");
}

/// Initialise objects for playing the game
void GameEngine::initPlayingObjects() {

	spike = manager->addObject<SpikeObject>(renderer, &playZone);

	player = manager->addObject<PlayerObject>(renderer, &playZone, spike, PLAYER1);

	manager->addObject<PlayerObject>(renderer, &playZone, player, PLAYER2);

	explosionImage = manager->addObject<ExplosionObject>(renderer);

	explosionImage->hide();
	spike->hide();

	addLife(); 
	addLife(); 
	addLife();


	SDL_Rect scorepos;
	scorepos.h = 48;
	scorepos.w = 100;
	scorepos.x = playZone.w - scorepos.w - 10;
	scorepos.y = 10;
	scoreText = manager->addObject<FontObject>(renderer, font, scorepos, WHITE, RIGHT);

	scoreText->setText(initString, BLACK);
	scoreText->hide();
	scorepos.y = 44;
	timerText = manager->addObject<FontObject>(renderer, font, scorepos, WHITE, RIGHT);
	timerText->setText(initString, BLACK);
	timerText->hide();

	scorepos.x -= 50;
	scorepos.y = 10;
	FontObject* temptext = manager->addObject<FontObject>(renderer, font, scorepos, WHITE, LEFT);
	temptext->setText("Score: ", BLACK);
	scorepos.y += 34;
	temptext = manager->addObject<FontObject>(renderer, font, scorepos, WHITE, LEFT);
	temptext->setText("Time: ", BLACK);

}


void GameEngine::allUpdate() {



	manager->update();

	timerText->setText(std::to_string(stageTimer.getMillis() / 1000));
	scoreText->setText(std::to_string(player->score));
	scoreText->show();
	timerText->show();

	auto bubbles = manager->getObjectTypeVector<BubbleObject>(Object_Bubble);

	for (auto& bubble : bubbles) {

		if (collidesWithCircle((player->render_rect), (bubble->render_rect))) {
			auto life = manager->getObjectTypeVector<GameObject>(Object_Lives);
			Mix_HaltChannel(1);
			player->getComponent<SoundHandler>()->play();
			if (life.size() > 1) {
				std::cout << "WE COLLIDIN'\n";
				SDL_Delay(1000);
				life[life.size() - 1]->destroy();
			}
			if (life.size() == 1) {
				std::cout << "WE DEAD!\n";
				life[life.size() - 1]->destroy();
			}

			if (manager->getObjectTypeVector<GameObject>(Object_Lives).size() > 1) {
				for (auto a : manager->getObjectTypeVector<BubbleObject>(Object_Bubble)) {
					a->destroy();
				}
				cleanObjects();
				spike->hide();
				for (int i = 0; i < 3; i++) {
					generateRandomBubble();
				}
				player->render_rect.x = playZone.x + playZone.w / 2 - player->render_rect.w / 2 - 5;
				player->getComponent<MovementHandler>()->setPosition(player->render_rect.x, player->render_rect.y);
				player->score = 0;
				stageTimer.start();
				unpause();
			}
			else {
				SDL_Rect a = { playZone.w / 2 - 24, playZone.h / 2 - 50, 100, 48 };
				auto text = manager->addObject<FontObject>(renderer, font, a, WHITE, CENTER);
				text->setText("You died! Press Enter to restart.", BLACK);
				pause();
			}

		}
	}

	if (spike->isVisible()) {

		for (auto& bubble : bubbles) {
			if (spike->isVisible() && collidesWithCircle((spike->render_rect), (bubble->render_rect))) {
				Mix_HaltChannel(1);
				bubble->getComponent<SoundHandler>()->play();

				explosionImage->show();

				explosionImage->render_rect = bubble->render_rect;
				explosionImage->getComponent<MovementHandler>()->setPosition(explosionImage->render_rect.x, explosionImage->render_rect.y);

				spike->hide();
				bubble->destroy();

				player->score++;
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
			if (manager->getObjectTypeVector<BubbleObject>(Object_Bubble).empty()) {
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

	manager->draw();

	SDL_RenderPresent(renderer);
}


/// Polls and handles all SDL events
void GameEngine::handleEvents() {

	SDL_PollEvent(&events);
	// User requests quit
	if (events.type == SDL_QUIT) {
		running = false;
	}
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	if (events.type == SDL_KEYDOWN) {
		if (currentKeyStates[SDL_SCANCODE_P] && paused) {
			unpause();
		}
		else if (currentKeyStates[SDL_SCANCODE_P]) {
			pause();
		}

		if (currentKeyStates[SDL_SCANCODE_RETURN] && paused) {
			setState(G_Infinite);
		}
		if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
			running = false;
		}
	}

}

void GameEngine::refresh() {
	pause();
	manager->freeAll();

}

void GameEngine::pause() {
	paused = true;
	stageTimer.pause();
	Mix_HaltChannel(1);
}

void GameEngine::unpause() {
	paused = false;
	stageTimer.unpause();
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
		refresh();
		initPlayingObjects();
		stageTimer.start();
		player->score = 0;
		unpause();
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
		static_cast<BubbleType>(randInt(1, 3)),
		randInt(0, playZone.w),
		randInt((int)(playZone.h / 3.0),
		(int)(playZone.h / 2.0)),
		randMinusPlus(),
		bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)].get()
	);
}

void inline GameEngine::addLife() {
	auto lives = manager->addObject(Object_Lives);
	lives->addComponent<MovementHandler>(0, 0);
	lives->addComponent<TileHandler>(renderer, heartTexture.get(), 1);
	lives->init();
	lives->getComponent<MovementHandler>()->setPosition((double)(manager->getObjectBaseVector(Object_Lives)->size() - 1) * lives->render_rect.w + 10, 10);
}

BubbleObject * GameEngine::addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture) {
	auto bubble = manager->addObject<BubbleObject>(type, posX, posY, direction, texture, renderer, &playZone, bubbleExplosion);
	return bubble;
}

