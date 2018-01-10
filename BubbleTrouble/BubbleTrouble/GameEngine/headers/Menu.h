#pragma once
#include <vector>
#include "ObjectManager.h"

enum MenuType{M_Main, M_Start, M_1Player, M_2Player, M_Options, M_Pause};

class BaseMenu {
private:
	MenuType type;
public:
	std::unique_ptr<ObjectManager> manager;

	BaseMenu(MenuType type) {
		manager = std::make_unique<ObjectManager>();
		this->type = type;
	}

	void init() {
		switch (type) {
		case M_Main:
			auto background = manager->addObject(Object_StaticImage);
			background->addComponent<TileHandler>();
			break;
		case M_Start:
			break;
		case M_1Player:
			break;
		case M_2Player:
			break;
		case M_Options:
			break;
		case M_Pause:
			break;
		default:
			break;
		}
	}

	void draw() {
		manager->draw();

	}

};


class MenuManager {
public:
	std::vector<BaseMenu> menu;
	
	void update() {

	}

	void draw() {
		if (!menu.empty()) {
			menu.back().draw();
		}
	}

	void pushMenu(MenuType type) {

	}

	void popMenu() {
		menu.pop_back();
	}
};