#include "GameEngine.h"
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/RandomInterface.h"
#include "headers/SoundHandler.h"
#include "headers/CollisionChecks.h"
#include <string>

#ifdef __linux__
#include <unistd.h>
#else
#include <windows.h>
#endif

SDL_Renderer * renderer;
SDL_Window * window;
TTF_Font * font;

/// Rectangle for the playingZone objects can be within
SDL_Rect playZone;


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
	setState(G_Menu);

	// Set render quality to 1, so that scaled objects are dithered a little
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

/// Free all allocated memory, hopefully.
GameEngine::~GameEngine() {
	refresh();
	TTF_CloseFont(font);
	menu->clearMenu();
	TTF_Quit();

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

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 4096) == -1) {
		std::cout << Mix_GetError();
	}
	TTF_Init();
	font = TTF_OpenFont("assets/FreeSansBold.ttf", 24);
	manager = std::make_unique<ObjectManager>();
	for (int i = 0; i < 4; i++) {
		TextureLoader * bubb = new TextureLoader("assets/WhiteBall_128x128.png");
		bubbleTextures.emplace_back(std::move(bubb));
		bubb->applyColor(colorarray[i]);
	}

	//std::unique_ptr<TextureLoader> Textl (new TextureLoader();
	heartTexture = std::make_unique<TextureLoader>("assets/heart.png");
	menu = std::make_unique<MenuManager>();

	bubbleExplosion = Mix_LoadWAV("assets/pop.wav");

	// Add entries to the keystates map
	keystates[SDL_SCANCODE_UP] = false;
	keystates[SDL_SCANCODE_DOWN] = false;
	keystates[SDL_SCANCODE_RETURN] = false;
	keystates[SDL_SCANCODE_ESCAPE] = false;
	keystates[SDL_SCANCODE_P] = false;
	keystates[SDL_SCANCODE_SPACE] = false;

}

/// Initialise objects for playing the game
void GameEngine::initPlayingObjects() {
	score = 0;

	if (currentState == G_Infinite_1Player) {
		auto spike = manager->addObject<SpikeObject>();
		manager->addObject<PlayerObject>(spike, SINGLEPLAYER);
	}
	else if (currentState == G_Infinite_2Player) {
		auto spike1 = manager->addObject<SpikeObject>();
		auto spike2 = manager->addObject<SpikeObject>();
		manager->addObject<PlayerObject>(spike1, PLAYER1);
		manager->addObject<PlayerObject>(spike2, PLAYER2);
	}

	for (auto& spike = manager->getObjectBaseVector(Object_Spike)->begin(); spike != manager->getObjectBaseVector(Object_Spike)->end(); spike++) {
		(*spike)->hide();
	}
	addLife(); 
	addLife(); 
	addLife();

	SDL_Rect scorepos;
	scorepos.h = 48;
	scorepos.w = 100;
	scorepos.x = playZone.w - scorepos.w - 10;
	scorepos.y = 10;
	scoreText = manager->addObject<FontObject>(font, scorepos, FontJustified_RIGHT);

	scoreText->setText(initString, BLACK);
	scoreText->hide();
	scorepos.y = 44;
	timerText = manager->addObject<FontObject>(font, scorepos, FontJustified_RIGHT);
	timerText->setText(initString, BLACK);
	timerText->hide();

	scorepos.x -= 50;
	scorepos.y = 10;
	FontObject* temptext = manager->addObject<FontObject>(font, scorepos, FontJustified_LEFT);
	temptext->setText("Score: ", BLACK);
	scorepos.y += 34;
	temptext = manager->addObject<FontObject>(font, scorepos, FontJustified_LEFT);
	temptext->setText("Time: ", BLACK);

}

void GameEngine::resetLevel() {
	if (manager->getObjectVector(Object_Lives).size() > 1) {
		for (auto a : manager->getObjectTypeVector<BubbleObject>(Object_Bubble)) {
			a->destroy();
		}
		for (auto & a : manager->getObjectTypeVector<PowerUpObject>(Object_PowerUp)) {
			a->destroy();
		}
		cleanObjects();
		for (auto& spike = manager->getObjectBaseVector(Object_Spike)->begin(); spike != manager->getObjectBaseVector(Object_Spike)->end(); spike++) {
			(*spike)->hide();
		}
		for (int i = 0; i < 3; i++) {
			generateRandomBubble();
		}

		if (currentState == G_Infinite_1Player) { 
			auto player = manager->getObjectBaseVector(Object_Player)->front().get();
			player->render_rect.x = playZone.x + playZone.w / 2 - player->render_rect.w / 2 - 5;
			player->getComponent<MovementHandler>()->setPosition(player->render_rect.x, player->render_rect.y);
		}
		else if (currentState == G_Infinite_2Player) {
			auto player1 = manager->getObjectBaseVector(Object_Player)->at(0).get();
			auto player2 = manager->getObjectBaseVector(Object_Player)->at(1).get();
			player1->getComponent<MovementHandler>()->setPosition(playZone.x + playZone.w / 4 - player1->render_rect.w / 2 - 5, player1->render_rect.y);
			player2->getComponent<MovementHandler>()->setPosition(playZone.x + 3 * playZone.w / 4 - player2->render_rect.w / 2 - 5, player2->render_rect.y);

		}
		score = 0;
		stageTimer.start();
		unpause();
	}
	else {
		SDL_Rect a = { playZone.w / 2 - 24, playZone.h / 2 - 50, 100, 48 };
		auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
		text->setText("You died! Press Enter to restart.", BLACK);
		pause();
	}
}

/// Method for condensing some code, updates each object while playing and do appropriate actions/processing.
void GameEngine::playLogicUpdate() {
	
	manager->update();

	timerText->setText(std::to_string(stageTimer.getMillis() / 1000));
	scoreText->setText(std::to_string(score));
	scoreText->show();
	timerText->show();

	auto bubbles = manager->getObjectTypeVector<BubbleObject>(Object_Bubble);

	for (auto & player = manager->getObjectBaseVector(Object_Player)->begin(); player != manager->getObjectBaseVector(Object_Player)->end(); player++) {
		for (auto & bubble : bubbles) {
			if (collidesWithCircle(((*player)->render_rect), (bubble->render_rect))) {
				auto life = manager->getObjectTypeVector<GameObject>(Object_Lives);
				Mix_HaltChannel(1);
				(*player)->getComponent<SoundHandler>()->play();
				if (life.size() > 1) {
					std::cout << "WE COLLIDIN'\n";
#ifdef __linux__
					nanosleep(1500 * 1000 * 1000);
#else
					Sleep(1500);
#endif
					life.back()->destroy();
				}
				if (life.size() == 1) {
					std::cout << "WE DEAD!\n";
					life.back()->destroy();
				}

				resetLevel();

			}
		}

		for (auto& a : manager->getObjectTypeVector<PowerUpObject>(Object_PowerUp)) {
			//the code is placed here because, once the spike hits the bubble, isVisible() = false, hence in the next update call the if (spike->isVisible()) is never entered.
			if (collidesWithRect((*player)->render_rect, a->render_rect)) {
				std::cout << "powerup gained\n";
				if (a->powerUpType == PU_Life && manager->getObjectBaseVector(Object_Lives)->size() <= 5) {
					addLife();
				}
				else if (a->powerUpType == PU_Coin)
					score += 5;

				a->destroy();
			}
		}
	}

	for (auto& spike = manager->getObjectBaseVector(Object_Spike)->begin(); spike != manager->getObjectBaseVector(Object_Spike)->end(); spike++) {
	
		if ((*spike)->isVisible()) {

			for (auto& bubble : bubbles) {
				if ((*spike)->isVisible() && collidesWithCircle(((*spike)->render_rect), (bubble->render_rect))) {
					Mix_HaltChannel(1);
					bubble->getComponent<SoundHandler>()->play();

					auto explosionImage = manager->addObject<ExplosionObject>();

					explosionImage->render_rect = bubble->render_rect;
					explosionImage->getComponent<MovementHandler>()->setPosition(explosionImage->render_rect.x, explosionImage->render_rect.y);

					//Add power ups functionality here. If the spike hits the bubble, randomly make a powerup fall from the place where the spike has hit the bubbles.
					//make sure the small bubbles doesn't contain any powerups
					//add more power ups..like faster spikes, faster player movement etcc..

					if ((randInt(1, 5) == 1) /* 20% chance */ && (bubble->bubbleType != Bubble0)) {
						std::size_t randint = randInt<std::size_t>(1, 10);
						PowerUpType PUtype;
						if (randint <= 3) PUtype = PU_Life; // 30% chance
						else PUtype = PU_Coin;

						auto powerUpObject = manager->addObject<PowerUpObject>(PUtype);
						powerUpObject->getComponent<MovementHandler>()->setPosition(bubble->render_rect.x, bubble->render_rect.y);
					}

					(*spike)->hide();
					bubble->destroy();

					score++;
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

	// Re-populate the board if all the bubbles are popped.
	if (manager->getObjectVector(Object_Bubble).empty()) {
		for (int i = 0; i < 3; i++) {
			generateRandomBubble();
		}
	}
}


/// Updates the game state, all objects.
void GameEngine::update() {

	if (!paused) {
		switch (currentState) {
		case G_Menu:
			break;
		case G_Infinite_1Player:

			playLogicUpdate();


			break;
		case G_Infinite_2Player:

			playLogicUpdate();

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
	if(menu->menu.empty()) manager->draw();
	else menu->draw();
	SDL_RenderPresent(renderer);
}

/// Polls and handles all SDL events
void GameEngine::handleEvents() {

	SDL_PollEvent(&events);
	// User requests quit
	if (events.type == SDL_QUIT) {
		running = false;
	}

	std::map<Uint8, bool> keyMap;
	for (auto key : keystates) {
		keyMap[key.first] = false;
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {
		// Only want to allow one action per keypress
		for (auto key : keystates) {
			if (!key.second && currentKeyStates[key.first]) {
				keyMap[key.first] = true;
				keystates[key.first] = true;
			}
		}
	}
	else if (events.type == SDL_KEYUP && events.key.repeat == 0) {
		// reset the keypress action if applicable

		for (auto key : keystates) {
			if (key.second && currentKeyStates[key.first] == 0) {
				keystates[key.first] = false;
			}
		}
	}

	if (currentState == G_Infinite_1Player || currentState == G_Infinite_2Player) {

		if (keyMap[SDL_SCANCODE_P] && paused && !manager->getObjectBaseVector(Object_Lives)->empty()) {
			unpause();
		}

		else if (keyMap[SDL_SCANCODE_P]) {
			pause();
		}

		if (keyMap[SDL_SCANCODE_RETURN] && paused) {
			setState(currentState);
		}
		if (keyMap[SDL_SCANCODE_ESCAPE]) {
			pause();
			setState(G_Menu);
		}
	}

	else if (currentState == G_Menu) {
		if (keyMap[SDL_SCANCODE_DOWN]) {
			menu->nextButton();
		}
		else if (keyMap[SDL_SCANCODE_UP]) {
			menu->previousButton();
		}

		if (keyMap[SDL_SCANCODE_RETURN] || keyMap[SDL_SCANCODE_SPACE]) {
			ButtonID ID = menu->activeButtonID();
			switch (ID) {
			case BID_PlayMode:
				menu->pushMenu(M_PlayMode);
				break;
			case BID_Options:
				menu->pushMenu(M_Options);
				break;
			case BID_Exit:
				running = false;
				break;
			case BID_Back:
				menu->popMenu();
				break;
			case BID_Infinite:
				menu->pushMenu(M_Infinite);
				break;
			case BID_Level:
				break;
			case BID_1Player:
				setState(G_Infinite_1Player);
				menu->menu.clear();
				break;
			case BID_2Player:
				setState(G_Infinite_2Player);
				menu->menu.clear();
				break;
			default:
				break;
			}
		}
		if (keyMap[SDL_SCANCODE_ESCAPE]) {
			running = false;
		}
	}
}


/// Pause the game and clear all objects
void GameEngine::refresh() {
	pause();
	manager->freeAll();
}

void GameEngine::pause() {
	paused = true;
	stageTimer.pause();
	Mix_HaltChannel(-1);
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
void GameEngine::setState(GameStates state) {
	currentState = state;

	switch (currentState) {
	case G_Menu:
		menu->pushMenu(M_Main);
		break;
		break;
	case G_Infinite_1Player:

		refresh();
		initPlayingObjects();

		stageTimer.start();
		score = 0;
		unpause();
		break;
	case G_Infinite_2Player:

		refresh();
		initPlayingObjects();

		stageTimer.start();
		score = 0;
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

/// Add a life to the board
void inline GameEngine::addLife() {
	auto lives = manager->addObject(Object_Lives);
	lives->addComponent<MovementHandler>(0, 0);
	lives->addComponent<TileHandler>(heartTexture.get(), 1);
	lives->init();
	// set the position in the succession
	lives->getComponent<MovementHandler>()->setPosition((double)(manager->getObjectBaseVector(Object_Lives)->size() - 1) * lives->render_rect.w + 10, 10);
}

/// 
BubbleObject * GameEngine::addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture) {
	auto bubble = manager->addObject<BubbleObject>(type, posX, posY, direction, texture, bubbleExplosion);
	return bubble;
}

