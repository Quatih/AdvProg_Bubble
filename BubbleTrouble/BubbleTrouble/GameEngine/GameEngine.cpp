#include "GameEngine.h"
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/RandomInterface.h"
#include "headers/SoundHandler.h"
#include "headers/CollisionChecks.h"
#include <string>
#include <fstream>

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
	playing = false;
	dead = false;
	end = false;
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
	keystates[SDL_SCANCODE_SPACE] = false;

}

/// Print the current score into the scoreboard file.
void GameEngine::fileHandling(void) {

	//std::time_t result = std::time(nullptr);
	std::ofstream myfile("scoreboard.txt", std::ios::app);
	if (myfile.is_open()) {
		if (score > 0) {
			myfile << score << std::endl;
			//myfile << std::asctime(std::localtime(&result)) << std::endl;
			//myfile << "    " << dt << std::endl;
			myfile.close();
		}
	}
}

/// Initialise objects for playing the game
void GameEngine::initPlayingObjects() {
	score = 0;
	if (currentState == G_Infinite_2Player) {
		auto spike1 = manager->addObject<SpikeObject>();
		auto spike2 = manager->addObject<SpikeObject>();
		manager->addObject<PlayerObject>(spike1, PLAYER1);
		manager->addObject<PlayerObject>(spike2, PLAYER2);

	}
	else {
		auto spike = manager->addObject<SpikeObject>();
		manager->addObject<PlayerObject>(spike, SINGLEPLAYER);
	}

	for (auto& spike : *manager->getObjectBaseVector(Object_Spike)) {
		spike->hide();
	}

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

	auto level = manager->addObject<FontObject>("assets/FreeSansBold.ttf", 40, SDL_Rect{ playZone.w / 2 - 25, 40, 50, 40 }, FontJustified_CENTER);
	level->setText(stateText[currentState], BLACK);
	playing = true;
	end = false;
	dead = false;

}

/// Reset all objects, for death purposes only
void GameEngine::resetLevel() {
	fileHandling();
	playing = true;
	end = false;
	dead = false;

	for (auto a : manager->getObjectTypeVector<BubbleObject>(Object_Bubble)) {
		a->destroy();
	}

	for (auto & a : manager->getObjectTypeVector<PowerUpObject>(Object_PowerUp)) {
		a->destroy();
	}
		
	cleanObjects();
		
	for (auto &spike : *(manager->getObjectBaseVector(Object_Spike))) {
		spike->hide();
	}

	if (currentState == G_Infinite_2Player) {
		auto player1 = manager->getObjectBaseVector(Object_Player)->at(0).get();
		auto player2 = manager->getObjectBaseVector(Object_Player)->at(1).get();
		player1->getComponent<MovementHandler>()->setPosition(playZone.x + playZone.w / 4 - player1->render_rect.w / 2 - 5, player1->render_rect.y);
		player2->getComponent<MovementHandler>()->setPosition(playZone.x + 3 * playZone.w / 4 - player2->render_rect.w / 2 - 5, player2->render_rect.y);
	}
	else {
		auto player = manager->getObjectBaseVector(Object_Player)->front().get();
		player->render_rect.x = playZone.x + playZone.w / 2 - player->render_rect.w / 2 - 5;
		player->getComponent<MovementHandler>()->setPosition(player->render_rect.x, player->render_rect.y);
	}
	manager->getObjectBaseVector(Object_Font)->back()->destroy();

	score = 0;
	manager->clean();
	setInitialBubbles();
	stageTimer.start();
	unpause();
}

/// Handle collision between the two objects.
bool GameEngine::handleCollision(GameObject * thing, GameObject * other) {
	bool collides = false;
	if (thing->type == Object_Bubble || other->type == Object_Bubble) {
		collides = collidesWithCircle(thing->render_rect, other->render_rect);
	}
	else collides = collidesWithRect(thing->render_rect, other->render_rect);

	if (collides) {
		if (other->type == Object_Player) {
			auto player = static_cast<PlayerObject*>(other);
			switch (thing->type) {
			case Object_Bubble:
				Mix_HaltChannel(1);
				player->getComponent<SoundHandler>()->play();

				removeLife(player->playerType);
				cleanObjects();
				if (manager->getObjectBaseVector(Object_Life_P1)->size() > 0 && (manager->getObjectBaseVector(Object_Life_P2)->size() > 0 || currentState != G_Infinite_2Player)) {
					std::cout << "WE COLLIDIN'\n";
					manager->addObject<FontObject>(font, SDL_Rect{ playZone.w / 2 - 50, playZone.h / 2 - 12, 100, 48 }, FontJustified_CENTER)->setText("You died!", BLACK);
					dead = true;
				}
				else {
					std::cout << "WE DEAD!\n";
					playing = false;
					dead = true;
					end = true;
					manager->addObject<FontObject>(font, SDL_Rect{ playZone.w / 2 - 50, playZone.h / 2 - 12, 100, 48 }, FontJustified_CENTER)->setText("Game Over!", BLACK);
				}
				pauseTimer.start();
				pause();
				break;
			case Object_PowerUp:
				std::cout << "powerup gained\n";
				if (static_cast<PowerUpObject*>(thing)->powerUpType == PU_Life && 
					((player->playerType == PLAYER1 || player->playerType == SINGLEPLAYER && manager->getObjectBaseVector(Object_Life_P1)->size() <= 5)
						||
					(player->playerType == PLAYER2 && manager->getObjectBaseVector(Object_Life_P1)->size() <= 5))) {
					addLife(player->playerType);
				}
				else if (static_cast<PowerUpObject*>(thing)->powerUpType == PU_Coin)
					score += 5;

				thing->destroy();

				break;
			default:
				break;
			}
		}
		else if (other->type == Object_Spike) {
			if (thing->type == Object_Bubble) {
				other->hide();
				thing->destroy();
				score++;

				Mix_HaltChannel(1);
				thing->getComponent<SoundHandler>()->play();
				auto explosionImage = manager->addObject<ExplosionObject>();

				explosionImage->render_rect = thing->render_rect;
				explosionImage->getComponent<MovementHandler>()->setPosition(explosionImage->render_rect.x, explosionImage->render_rect.y);

				//Add power ups functionality here. If the spike hits the bubble, randomly make a powerup fall from the place where the spike has hit the bubbles.
				//make sure the small bubbles doesn't contain any powerups
				//add more power ups..like faster spikes, faster player movement etcc..

				if ((randInt(1, 5) == 1) /* 20% chance */ && (dynamic_cast<BubbleObject*>(thing)->bubbleType != Bubble0)) {
					std::size_t randint = randInt<std::size_t>(1, 10);
					PowerUpType PUtype;
					if (randint <= 3) PUtype = PU_Life; // 30% chance
					else PUtype = PU_Coin;

					auto powerUpObject = manager->addObject<PowerUpObject>(PUtype);
					powerUpObject->getComponent<MovementHandler>()->setPosition(thing->render_rect.x, thing->render_rect.y);
				}

				std::cout << "Bubble popped\n";
				if (dynamic_cast<BubbleObject*>(thing)->pops > 0) {
					std::size_t cindex = randInt<std::size_t>(0, bubbleTextures.size() - 1);
					auto temp = addBubble(dynamic_cast<BubbleObject*>(thing)->getNextBubble(), thing->render_rect.x, thing->render_rect.y, 1, bubbleTextures[cindex].get());
					temp->getComponent<MovementHandler>()->velocity.y = -abs(thing->getComponent<MovementHandler>()->baseVelocity.y)*0.6;

					temp = addBubble(dynamic_cast<BubbleObject*>(thing)->getNextBubble(), thing->render_rect.x, thing->render_rect.y, -1, bubbleTextures[cindex].get());
					temp->getComponent<MovementHandler>()->velocity.y = -abs(thing->getComponent<MovementHandler>()->baseVelocity.y)*0.6;
				}
			}
		}
	}
	return collides;
}

/// Method for condensing some code, updates each object while playing and do appropriate actions/processing.
void GameEngine::playLogicUpdate() {

	manager->update();

	std::string timerstr = std::to_string(stageTimer.getMillis() / 1000);
	timerText->setText(timerstr, timerText->color);
	
	std::string scorestr = std::to_string(score);
	scoreText->setText(scorestr, scoreText->color);
	scoreText->show();
	timerText->show();

	for (auto& player : *(manager->getObjectBaseVector(Object_Player))) {
		for (auto& bubble: *(manager->getObjectBaseVector(Object_Bubble))) {
			if (handleCollision(bubble.get(), player.get())) break;
		}

		for (auto& powerup : *(manager->getObjectBaseVector(Object_PowerUp))) {
			//the code is placed here because, once the spike hits the bubble, isVisible() = false, hence in the next update call the if (spike->isVisible()) is never entered.
			handleCollision(powerup.get(), player.get());
		}
	}
	for (auto& spike : *(manager->getObjectBaseVector(Object_Spike))) {
	
		if (spike->isVisible()) {

			for (auto& bubble : *(manager->getObjectBaseVector(Object_Bubble))) {
				if (handleCollision(bubble.get(), spike.get())) break;
			}
		}
	}

}

/// Updates the game state, all objects.
void GameEngine::update() {

	if (!paused && pauseTimer.paused) {
		switch (currentState) {
		case G_Menu:
			break;
		case G_Infinite_1Player:
			playLogicUpdate();

			setInitialBubbles();

			break;
		case G_Infinite_2Player:
			playLogicUpdate();

			setInitialBubbles();

			break;
		case G_Level1:
			playLogicUpdate();
			if (manager->getObjectVector(Object_Bubble).empty()) {

				end = true;
				pauseTimer.start();
				SDL_Rect a = { playZone.w / 2 - 50, playZone.h / 2 - 24, 100, 24 };
				auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
				text->setText("Level 1 completed", BLACK);
				text->show();

			}
			break;
		case G_Level2:
			playLogicUpdate();
			if (manager->getObjectVector(Object_Bubble).empty()) {

				end = true;
				pauseTimer.start();
				SDL_Rect a = { playZone.w / 2 - 50, playZone.h / 2 - 24, 100, 24 };
				auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
				text->setText("Level 2 completed", BLACK);
				text->show();
			}
			break;
		case G_Level3:
			playLogicUpdate();
			if (manager->getObjectVector(Object_Bubble).empty()) {
				end = true;
				pauseTimer.start();
				SDL_Rect a = { playZone.w / 2 - 50, playZone.h / 2 - 24, 100, 24 };
				auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
				text->setText("Level 3 completed", BLACK);
				text->show();
			}
			break;
		case G_Level4:
			playLogicUpdate();
			if (manager->getObjectVector(Object_Bubble).empty()) {
				end = true;
				pauseTimer.start();
				SDL_Rect a = { playZone.w / 2 - 50, playZone.h / 2 - 24, 100, 24 };
				auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
				text->setText("Level 4 completed", BLACK);
				text->show();
			}
			break;
		case G_Level5:
			playLogicUpdate();
			if (manager->getObjectVector(Object_Bubble).empty()) {
				end = true;
				pauseTimer.start();
				SDL_Rect a = { playZone.w / 2 - 50, playZone.h / 2 - 24, 100, 24};
				auto text = manager->addObject<FontObject>(font, a, FontJustified_CENTER);
				text->setText("Level 5 completed, you've won!", BLACK);
				text->show();
			}
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
	else if (!pauseTimer.paused) {
		if (currentState == G_Level5 || currentState == G_Infinite_1Player || currentState == G_Infinite_2Player) {
			// end of level reached
			if (pauseTimer.getMillis() > 2000 && end) {
				pauseTimer.reset();
				pause();
				setState(G_Menu);
			}
			// lives are gone
			else if (pauseTimer.getMillis() > 1000 && end && dead) {
				pause();
				pauseTimer.reset();
				setState(G_Menu);
			}
			// died
			else if (pauseTimer.getMillis() > 1000 && dead ) {

				pauseTimer.reset();
				resetLevel();
			}
		}
		else {

			// end of level reached
			if (pauseTimer.getMillis() > 2000 && end ) {
				pauseTimer.reset();
				setState(static_cast<GameStates>(currentState + 1));
			}

			// lives are gone
			else if (pauseTimer.getMillis() > 1000 && dead && !playing) {
				pause();
				pauseTimer.reset();
				setState(G_Menu);
			}
			// died
			else if (pauseTimer.getMillis() > 1000 && dead ) {

				pauseTimer.reset();
				resetLevel();
			}
		}	
	}
}

/// Render each object on the screen.
void GameEngine::render() {
	SDL_RenderClear(renderer);
	if(currentState != G_Menu) manager->draw();
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

	// If we're in the menu
	if (currentState == G_Menu) {
		// Next or previous button
		if (keyMap[SDL_SCANCODE_DOWN]) {
			menu->nextButton();
		}
		else if (keyMap[SDL_SCANCODE_UP]) {
			menu->previousButton();
		}

		// If a button is selected
		if (keyMap[SDL_SCANCODE_RETURN] || keyMap[SDL_SCANCODE_SPACE]) {
			ButtonID ID = menu->activeButtonID();
			switch (ID) {
			case BID_PlayMode:
				menu->pushMenu(M_PlayMode);
				break;
			case BID_Options:
				menu->pushMenu(M_Options);
				break;
			case BID_Quit:
				running = false;
				fileHandling();
				break;
			case BID_Back:
				menu->popMenu();
				break;
			case BID_Infinite:
				menu->pushMenu(M_Infinite);
				break;
			case BID_Level:
				menu->pushMenu(M_Level);
				break;
			case BID_Level1:
				playing = false;
				setState(G_Level1);
				menu->menu.clear();
				break;
			case BID_Level2:
				playing = false;
				setState(G_Level2);
				menu->menu.clear();
				break;
			case BID_Level3:
				playing = false;
				setState(G_Level3);
				menu->menu.clear();
				break;
			case BID_Level4:
				playing = false;
				setState(G_Level4);
				menu->menu.clear();
				break;
			case BID_Level5:
				playing = false;
				setState(G_Level5);
				menu->menu.clear();
				break;
			case BID_1Player:
				playing = false;
				setState(G_Infinite_1Player);
				menu->menu.clear();
				break;
			case BID_2Player:
				playing = false;
				setState(G_Infinite_2Player);
				menu->menu.clear();
				break;
			case BID_Volume:
				menu->pushMenu(M_Volume);
				break;
			case BID_Mute:
				svolume = 0;
				Mix_Volume(-1, svolume);
				menu->popMenu();
				menu->popMenu();
				break;
			case BID_Min:
				svolume = 25;
				Mix_Volume(-1, svolume);
				menu->popMenu();
				menu->popMenu();
				break;
			case BID_Med:
				svolume = 50;
				Mix_Volume(-1, svolume);
				menu->popMenu();
				menu->popMenu();
				break;
			case BID_Max:
				svolume = 128;
				Mix_Volume(-1, svolume);
				menu->popMenu();
				menu->popMenu();
				break;
			case BID_High_Scores:
				menu->pushMenu(M_HighScore);
				break;
			case BID_Continue:

				menu->clearMenu();
				currentState = previousState;
				unpause();

				break;
			default:
				break;
			}
		}
		if (keyMap[SDL_SCANCODE_ESCAPE]) {
			if (menu->menu.size() > 1) menu->popMenu();
			else if (playing) {
				menu->clearMenu();
				currentState = previousState;
				unpause();

			}
			else {
				running = false;
				fileHandling();
			}
		}
	}
	// If we're in playmode
	else {

		// Set to menu
		if (keyMap[SDL_SCANCODE_ESCAPE]) {
			pause();
			setState(G_Menu);
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
	previousState = currentState;
	currentState = state;
	if (state != G_Menu) {
		fileHandling();
		auto lives = manager->getObjectBaseVector(Object_Life_P1)->size();

		refresh();

		if (currentState == G_Infinite_1Player) {
			addLife(SINGLEPLAYER);
			addLife(SINGLEPLAYER);
			addLife(SINGLEPLAYER);
		}
		else if (currentState == G_Infinite_2Player){
			for (auto i = 0; i < 3; i++) {
				addLife(PLAYER1);
			}
			for (auto i = 0; i < 3; i++) {
				addLife(PLAYER2);
			}
		}
		else if(playing) {
			
			for (auto i = 0; i < lives; i++) {
				addLife(SINGLEPLAYER);
			}
		}
		else {
			for (auto i = 0; i < 3; i++) {
				addLife(SINGLEPLAYER);
			}
		}

		initPlayingObjects();

		stageTimer.start();
		pauseTimer.reset();
		unpause();
		setInitialBubbles();
		playing = true;
	}
	else {
		if (playing) menu->pushMenu(M_Paused);
		else menu->pushMenu(M_Main);
	}
}

/// Initiate the bubbles for the current state
void GameEngine::setInitialBubbles(){
	switch (currentState) {
	case G_Menu:
		break;
	case G_Infinite_1Player:
		if (manager->getObjectVector(Object_Bubble).empty()) {
			for (int i = 0; i < 3; i++) {
				generateRandomBubble();

			}
			
		}
		break;
	case G_Infinite_2Player:
		if (manager->getObjectVector(Object_Bubble).empty()) {
			for (int i = 0; i < 3; i++) {
				generateRandomBubble();
			}
		}
		break;
	case G_Level1:

		generateRandomBubble(Bubble2);

		break;
	case G_Level2:
		for (int i = 0; i < 2; i++) {
			generateRandomBubble(Bubble2);
		}
		break;
	case G_Level3:
		for (int i = 0; i < 2; i++) {
			generateRandomBubble(Bubble3);
		}
		break;
	case G_Level4:

		for (int i = 0; i < 2; i++) {
			generateRandomBubble(Bubble3);
		}
		generateRandomBubble(Bubble4);
	
		break;
	case G_Level5:
		for (int i = 0; i < 3; i++) {
			generateRandomBubble(Bubble4);
		}
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

/// Generate a randomly positioned bubble of a specific type
void inline GameEngine::generateRandomBubble(BubbleType type) {
	addBubble(
		type,
		randInt(0, playZone.w),
		randInt((int)(playZone.h / 3.0),
		(int)(playZone.h / 2.0)),
		randMinusPlus(),
		bubbleTextures[randInt<std::size_t>(0, bubbleTextures.size() - 1)].get()
	);
}

/// Add a life for the correct player
void GameEngine::removeLife(PlayerNumber playerNum) {
	if (playerNum == PLAYER2) {
		if (!manager->getObjectBaseVector(Object_Life_P2)->empty()) manager->getObjectBaseVector(Object_Life_P2)->back()->destroy();
	}
	else {
		if (!manager->getObjectBaseVector(Object_Life_P1)->empty()) manager->getObjectBaseVector(Object_Life_P1)->back()->destroy();
	}
}

/// Add a life to the board
void inline GameEngine::addLife(PlayerNumber playerType) {
	if (playerType == PLAYER2) {
		auto lives = manager->addObject(Object_Life_P2);
		lives->addComponent<MovementHandler>(0, 0);
		lives->addComponent<TileHandler>(heartTexture.get(), 1);
		lives->init();
		// set the position in the succession
		lives->getComponent<MovementHandler>()->setPosition((double)(manager->getObjectBaseVector(Object_Life_P2)->size() - 1) * lives->render_rect.w + 10, 60);
	}
	else {
		auto lives = manager->addObject(Object_Life_P1);
		lives->addComponent<MovementHandler>(0, 0);
		lives->addComponent<TileHandler>(heartTexture.get(), 1);
		lives->init();
		// set the position in the succession
		lives->getComponent<MovementHandler>()->setPosition((double)(manager->getObjectBaseVector(Object_Life_P1)->size() - 1) * lives->render_rect.w + 10, 10);

	}
}

/// Add a bubble 
BubbleObject * GameEngine::addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture) {
	auto bubble = manager->addObject<BubbleObject>(type, posX, posY, direction, texture, bubbleExplosion);
	return bubble;
}

