#pragma once
#include "GameObject.h"
#include "TileHandler.h"
#include <vector>
#include <fstream>

class GameObject;

class Map {
private:
	std::vector<GameObject*> maptiles;

public:
	Map() {

	}
	void update() {
		for (GameObject* tile : maptiles) {
			tile->update();
		}
	}

	void draw() {
		for (GameObject* tile : maptiles) {
			tile->draw();
		}
	}

	//void loadMap(std::string path, int x, int y) {
	//	/*char tileType;
	//	std::fstream file;
	//	file.open(path);

	//	for (int j = 0; j < y; j++) {
	//		for (int i = 0; i < x; i++) {
	//			
	//		}
	//	}*/

	//}

};