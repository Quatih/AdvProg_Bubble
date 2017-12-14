#pragma once
#include "headers/GameObject.h"
#include "headers/Components.h"
#include <vector>
#include <random>
#include <ctime>



enum GameState {G_Init, G_Menu, G_MenuOptions, G_LevelSelect,  G_Infinite, G_Level1, G_Level2};

/// Handles all the game logic
class GameEngine {
private:
	GameState currentState;
	GameObject * player;
	std::vector<GameObject*> bubbles;
	GameObject * spike;

	SDL_Window * window;
	SDL_Event events;
	bool running;

	/// Rectangle for the playingZone objects can be within
	SDL_Rect playZone;
	/// The zone the Spike can be within
	SDL_Rect spikeZone;

	/// Used for random access of color.
	const Color colorarray[4] = { RED, GREEN, BLUE, BLACK };

	/// Use this to re-use the bubble textures and minimize memory allocation.
	std::vector<TextureLoader*> bubbleTextures;

public:
	SDL_Renderer * renderer;

	/// Constructor creates the window and renderer
	GameEngine(std::string title, int winposx, int winposy, int winwidth, int winheight, SDL_WindowFlags flag);;

	/// Free all allocated memory, hopefully.
	~GameEngine();;

	bool inline isRunning() const {
		return running;
	} 


	/// Initialize player, spike and bubbles.
	void init();

	/// Updates the game state, all objects.
	void update();

	/// Render each object on the screen.
	void render();

	/// Polls and handles all SDL events
	void handleEvents();

	/// Deletes invalidated game objects
	void cleanObjects();


	void start();

	/// Generate a random bubble
	void inline generateRandomBubble();

	/// Add a bubble to the bubble vector and initialize.
	GameObject * addBubble(int radius, int posX, int posY, float velocityX, float velocityY, float acceleration, int pops, TextureLoader * texture);
};