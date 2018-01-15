#pragma once
#include "headers/ObjectManager.h"
#include "headers/BubbleObject.h"
#include "headers/PlayerObject.h"
#include "headers/SpikeObject.h"
#include "headers/ExplosionObject.h"
#include "headers/GameObject.h"
#include "headers/Components.h"
#include "headers/FontObject.h"
#include "headers/MillisTimer.h"
#include "headers/PowerUpObject.h"
#include "headers/Menu.h"
#include <vector>	
#include <random>
#include <ctime>
#include <map>

const std::string initString = "string";

/// Different states for each different presentation 
enum GameStates : std::size_t { G_Menu, G_Infinite_1Player, G_Infinite_2Player,
	G_Level1, G_Level2, G_Level3, G_Level4, G_Level5, G_Level6, G_Level7, G_Level8, G_Level9, G_Level10
};

/// Handles all the game logic
class GameEngine {
private:
	GameStates currentState;

	std::unique_ptr<ObjectManager> manager;

	FontObject* scoreText;
	FontObject* timerText;

	SDL_Event events;
	bool running, paused;

	/// Re-use the explosion audio clip
	Mix_Chunk * bubbleExplosion;

	/// Used for random access of color.
	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };

	/// Use this to re-use the bubble textures and minimize memory allocation.
	std::vector<std::unique_ptr<TextureLoader>> bubbleTextures;
	/// Re-use heart Texture
	std::unique_ptr<TextureLoader> heartTexture;
	/// Class for stage timing
	MillisTimer stageTimer;
	std::unique_ptr<MenuManager> menu;


public:
	std::map<Uint8, bool> keystates;
	int score = 0;
	/// Constructor creates the window and renderer
	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag);;

	/// Free all allocated memory, hopefully.
	~GameEngine();

	bool inline isRunning() const {
		return running;
	} 

	/// Initialize player, spike and bubbles.
	void init();

	/// Initialize player, spike and bubbles.
	void initPlayingObjects();

	/// Updates the game state, all objects.
	void update();

	/// Render each object on the screen.
	void render();

	/// Polls and handles all SDL events
	void handleEvents();

	/// Clear all the objects in the objectmanager and pause the game
	void refresh();

	/// Deletes invalidated game objects
	void cleanObjects();

	/// Resets the current playing level
	void resetLevel();

	/// Main gameplay update function
	void playLogicUpdate();

	/// Pause the game, the timer in use
	void pause();

	/// Unpause the game, turn on the timer
	void unpause();

	/// Add a life to the board
	void addLife();

	/// Set the State of the game, takes appropriate action.
	void setState(GameStates state);

	/// Generate a random bubble
	void inline generateRandomBubble();

	void fileHandling(void);
	void readScores(void);

	/// Add a bubble to the bubble vector and initialize.
	BubbleObject * addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture);
	//BubbleObject * addBubble(int radius, int posX, int posY, double velocityX, double velocityY, double acceleration, int pops, TextureLoader * texture);
};	