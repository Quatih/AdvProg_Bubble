#pragma once
#include <vector>
#include "ObjectManager.h"
#include "Components.h"

enum MenuType{M_Main, M_Start, M_1Player, M_2Player, M_Options, M_Pause};
extern SDL_Window * window;
extern TTF_Font * font;


class BaseMenu {
private:
	MenuType type;
public:
	std::unique_ptr<GameObject> backgroundObject;
	TextureLoader * buttonTexture;
	TextureLoader * activeButtonTexture;
	Uint64 activeButton = 0;

	std::vector < std::pair<std::unique_ptr<GameObject>, std::unique_ptr<GameObject>>> buttons;

	BaseMenu(MenuType type, TextureLoader * button, TextureLoader * activeButton) {
		buttonTexture = button;
		activeButtonTexture = activeButton;
		this->type = type;
		init();
	}

	~BaseMenu() {
		buttons.clear();

	}

	void init() {
		backgroundObject = std::make_unique<GameObject>(Object_StaticImage);
		backgroundObject->addComponent<TileHandler>("assets/square.png", 1);
		backgroundObject->init();
		backgroundObject->getComponent<TileHandler>()->applyColor({ 200, 0, 0 });
		int h;
		int w;
		SDL_GetWindowSize(window, &w, &h);
		backgroundObject->render_rect.h = h;
		backgroundObject->render_rect.w = w;

		switch (type) {
		case M_Main:
			addButton("Play");
			addButton("Options");
			addButton("Exit");
			setActiveButton(0);

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

	void addButton(std::string text) {
		GameObject * button;
		FontObject * fontobject;

		button = new GameObject(Object_StaticImage);
		button->addComponent<TileHandler>(buttonTexture, 0.8);
		button->addComponent<MovementHandler>(0, 0);

		button->init();
		button->getComponent<MovementHandler>()->setPosition(double(backgroundObject->render_rect.w / 2 - button->render_rect.w / 2), 
					(double)(backgroundObject->render_rect.h / 2 - button->render_rect.h / 2 + buttons.size()*(button->render_rect.h + 10)));
		SDL_Rect rect = button->render_rect;
		fontobject = new FontObject(font, rect, WHITE, CENTER);
		fontobject->setText(text);
		buttons.emplace_back(std::make_pair(std::move(button), std::move(fontobject)));
	}

	void nextButton() {
		if (activeButton < buttons.size() - 1) {
			buttons.at(activeButton).first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton + 1);
		}
	}

	void previousButton() {
		
		if (activeButton > 0) {
			buttons.at(activeButton).first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton - 1);
		}
	}

	void setActiveButton(Uint64 number) {
		buttons.at(number).first->getComponent<TileHandler>()->setTextureLoader(activeButtonTexture);
		activeButton = number;
	}

	void draw() {
		backgroundObject->draw();
		for (auto& a : buttons) {
			a.first->draw();
			a.second->draw();
		}
	}

};


class MenuManager {
public:
	std::vector<std::unique_ptr<BaseMenu>> menu;


	std::unique_ptr<TextureLoader> buttonTexture;
	std::unique_ptr<TextureLoader> activeButtonTexture;

	MenuManager() {
		buttonTexture = std::make_unique<TextureLoader>("assets/MenuItem.png");
		activeButtonTexture = std::make_unique<TextureLoader>("assets/MenuSelected.png");
	}

	void nextButton() {
		menu.back()->nextButton();
	}

	void previousButton() {
		menu.back()->previousButton();
	}

	void activateButton() {

	}

	void draw() {
		if (!menu.empty()) {
			menu.back()->draw();
		}
	}

	void pushMenu(MenuType type) {
		auto pushedmenu = new BaseMenu(type, buttonTexture.get(), activeButtonTexture.get());

		menu.emplace_back(std::move(pushedmenu));
	}

	void popMenu() {
		if(!menu.empty()) menu.pop_back();
	}
};