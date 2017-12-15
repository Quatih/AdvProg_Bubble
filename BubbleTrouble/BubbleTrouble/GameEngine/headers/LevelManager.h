#pragma once
#include <iostream>
#include <fstream>

class LevelManager {
	std::string filePath;
	std::ifstream levelFile;
public:

	LevelManager(std::string path) {
		filePath = path;
	}

	void init() {
		levelFile.open(filePath);
		if(!levelFile) std::cout << "Level File failed to open\n";
	}

	void nextLevel() {

	}

};