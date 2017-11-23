#pragma once
#include "GameObject.h"
#include "GameComponent.h"
#include "Components.h"
#include "Scoreboard.h"
#include "SoundPlayer.h"
#include <vector>
#include "Map.h"
#include "Menu.h"

class GameObject;
class CollisionHandler;

class GameEngine {
		GameObject * player;
		std::vector<GameComponent> components;
		
		GameEngine(){

		};
		~GameEngine(){
		};
};