#pragma once
#include <vector>
#include "ObjectManager.h"
#include "Components.h"
#include <map>


enum MenuType {M_Main, M_PlayMode, M_Infinite, M_Options, M_Volume};
extern SDL_Window * window;
extern TTF_Font * font;
enum ButtonID : Uint64 {BID_PlayMode, BID_Options, BID_Exit, BID_Back, BID_Infinite, BID_Level, BID_1Player, BID_2Player, BID_Volume, BID_Min, BID_Max, BID_Mute
};
const std::string buttonText[] = {"Play", "Options", "Quit", "Back", "Infinite Mode", "Level Mode", "1 Player", "2 Player", "Set Volume", "Minimum", "Maximum" , "Mute" };

/// Menu class that keeps track of all objects in the menu
class BaseMenu {
private:
	MenuType type;
public:
	std::unique_ptr<GameObject> backgroundObject;
	TextureLoader * buttonTexture;
	TextureLoader * activeButtonTexture;
	std::unique_ptr<FontObject> titleText;
	/// Active button index in the buttonIDs vector.
	Uint64 activeButton = 0;
	/// Map using the buttonIds to store a pair of objects, a button image and font
	std::map <ButtonID, std::pair<std::unique_ptr<GameObject>, std::unique_ptr<FontObject>>> buttons;
	std::vector<ButtonID> buttonIDs;

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
		backgroundObject->getComponent<TileHandler>()->applyColor({ 180, 0, 0 });
		int h;
		int w;
		SDL_GetWindowSize(window, &w, &h);
		backgroundObject->render_rect.h = h;
		backgroundObject->render_rect.w = w;
		SDL_Rect titleRect;
		titleRect.h = 50;
		titleRect.w = 10;
		titleRect.x = backgroundObject->render_rect.w / 2 - titleRect.w/2;
		titleRect.y = backgroundObject->render_rect.h / 4 - titleRect.h/2;

		titleText = std::make_unique<FontObject>("assets/FreeSansBold.ttf", 80, titleRect, FontJustified_CENTER);
		titleText->setText("Bubble Trouble", BLACK);


		switch (type) {
		case M_Main:
			addButton(BID_PlayMode);
			addButton(BID_Options);
			addButton(BID_Exit);
			
			break;
		case M_PlayMode:
			//addButton(BID_Level);
			addButton(BID_Infinite);
			addButton(BID_Back);
			break;
		case M_Infinite:
			addButton(BID_1Player);
			addButton(BID_2Player);
			addButton(BID_Back);
			break;
		case M_Options:
			addButton(BID_Volume);
			addButton(BID_Back);
			break;
		case M_Volume:
			addButton(BID_Min);
			addButton(BID_Max);
			addButton(BID_Mute);
			addButton(BID_Back);
			break;
		default:
			break;
		}
		setActiveButton(0);
	}

	/// Add a button to the map with the specific ID.
	void addButton(ButtonID ID) {
		GameObject * button;
		FontObject * fontobject;

		button = new GameObject(Object_StaticImage);
		button->addComponent<TileHandler>(buttonTexture, 0.8);
		button->addComponent<MovementHandler>(0, 0);

		button->init();
		button->getComponent<MovementHandler>()->setPosition(double(backgroundObject->render_rect.w / 2 - button->render_rect.w / 2),
			(double)(backgroundObject->render_rect.h / 2 - button->render_rect.h / 2 + buttons.size()*(button->render_rect.h + 10)));
		SDL_Rect rect = button->render_rect;
		fontobject = new FontObject(font, rect, FontJustified_CENTER);
		fontobject->setText(buttonText[ID]);
		std::unique_ptr<GameObject> uButton { button };
		std::unique_ptr<FontObject> uFont { fontobject};

		buttons[ID] = std::make_pair(std::move(uButton), std::move(uFont));
		buttonIDs.push_back(ID);
	}

	/// Set the next button as the active button
	void nextButton() {
		if (activeButton < buttonIDs.size() - 1) {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton + 1);
		}
	}

	/// Set the previous button as the active button
	void previousButton() {
		
		if (activeButton > 0) {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton - 1);
		}
	}

	/// Set a soecufuc button as the active button
	void setActiveButton(Uint64 number) {
		buttons[buttonIDs[number]].first->getComponent<TileHandler>()->setTextureLoader(activeButtonTexture);
		activeButton = number;
	}

	void draw() {
		backgroundObject->draw();
		for (auto& a : buttonIDs) {
			buttons[a].first->draw();
			buttons[a].second->draw();
		}
		titleText->draw();
	}
};

/// Keeps track of the menu overlays
class MenuManager {
public:
	/// The Menu stack
	std::vector<std::unique_ptr<BaseMenu>> menu;

	std::unique_ptr<TextureLoader> buttonTexture;
	std::unique_ptr<TextureLoader> activeButtonTexture;

	MenuManager() {
		buttonTexture = std::make_unique<TextureLoader>("assets/MenuItem.png");
		activeButtonTexture = std::make_unique<TextureLoader>("assets/MenuSelected.png");
	}

	/// Set the next button below
	void nextButton() {
		menu.back()->nextButton();
	}

	/// Set the previous button above
	void previousButton() {
		menu.back()->previousButton();
	}

	/// Returns which button is currently active or highlighted
	ButtonID activeButtonID() {
		return menu.back()->buttonIDs[menu.back()->activeButton];
	}

	void draw() {
		// We only want to draw top of the stack if it has any items
		if (!menu.empty()) {
			menu.back()->draw();
		}
	}

	/// Add a new menu of the type to the stack
	void pushMenu(MenuType type) {
		BaseMenu * pushedmenu = new BaseMenu(type, buttonTexture.get(), activeButtonTexture.get());
		menu.emplace_back(std::move(pushedmenu));
	}

	/// Pop a menu from the stack.
	void popMenu() {
		if(!menu.empty()) menu.pop_back();
	}

	/// Clear all the menus
	void clearMenu() {
		while (!menu.empty()) {
			menu.pop_back();
		}
	}

};