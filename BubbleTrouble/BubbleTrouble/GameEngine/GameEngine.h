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
#include <vector>	
#include <random>
#include <ctime>

const std::string initString = "string";

/// Different states for each different presentation 
enum GameState {G_Init, G_Menu, G_MenuOptions, G_LevelSelect, G_Infinite, 
	G_Level1, G_Level2, G_Level3, G_Level4, G_Level5, G_Level6, G_Level7, G_Level8, G_Level9, G_Level10
};

/// Handles all the game logic
class GameEngine {
private:
	GameState currentState;

	std::unique_ptr<ObjectManager> manager;

	PlayerObject* player;
	SpikeObject* spike;
	FontObject* scoreText;
	FontObject* timerText;
	ExplosionObject* explosionImage;
	
	SDL_Window * window;

	SDL_Event events;
	bool running, paused;

	/// Re-use the explosion audio clip
	Mix_Chunk * bubbleExplosion;
	/// Rectangle for the playingZone objects can be within
	SDL_Rect playZone;
	/// The zone the Spike can be within
	SDL_Rect spikeZone;

	/// Used for random access of color.
	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };

	/// Use this to re-use the bubble textures and minimize memory allocation.
	std::vector<std::unique_ptr<TextureLoader>> bubbleTextures;
	/// Re-use heart Texture
	std::unique_ptr<TextureLoader> heartTexture;
	/// Class for stage timing
	MillisTimer stageTimer;
	TTF_Font * font;

public:

	SDL_Renderer * renderer;

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

	/// Main gameplay update function
	void allUpdate();

	/// Pause the game, the timer in use
	void pause();

	/// Unpause the game, turn on the timer
	void unpause();

	/// Add a life to the board
	void addLife();

	/// Set the State of the game, takes appropriate action.
	void setState(GameState state);

	/// Generate a random bubble
	void inline generateRandomBubble();

	/// Add a bubble to the bubble vector and initialize.
	BubbleObject * addBubble(BubbleType type, int posX, int posY, int direction, TextureLoader * texture);
	//BubbleObject * addBubble(int radius, int posX, int posY, double velocityX, double velocityY, double acceleration, int pops, TextureLoader * texture);
};	