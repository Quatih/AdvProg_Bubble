#pragma once
#include "GameObject.h"
#include "GameComponent.h"
#include "Components.h"
#include "Scoreboard.h"
#include "SoundPlayer.h"
#include <vector>
#include "Menu.h"
#include "Map.h"

class GameEngine {
	GameObject * player;
	std::vector<GameObject> objects;
	Menu * menu;
	SoundPlayer * sPlayer;
	Scoreboard * scoreboard;
	Map * map;
		
	GameEngine(){
	};
	~GameEngine(){
			
	};
};