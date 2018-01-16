#pragma once
#include <vector>
#include "ObjectManager.h"
#include "Components.h"
#include <map>
#include <fstream>

enum MenuType {M_Main, M_PlayMode, M_Infinite, M_Level, M_Options, M_Volume, M_HighScore, M_Paused};
extern SDL_Window * window;
extern TTF_Font * font;
enum ButtonID : Uint64 {BID_PlayMode, BID_Options, BID_Quit, BID_Back, BID_Infinite, 
	BID_Level, BID_1Player, BID_2Player, BID_Volume, BID_Min, BID_Med, BID_Max, BID_Mute, BID_High_Scores, BID_Continue, BID_Level1, BID_Level2, BID_Level3, BID_Level4, BID_Level5
};
static const std::string buttonText[] = {"Play", "Options", "Quit", "Back", "Infinite Mode", 
	"Level Mode", "1 Player", "2 Player", "Volume", "Minimum", "Medium", "Maximum", "Mute", "High Scores", "Continue", "Level1","Level2" ,"Level3" ,"Level4" ,"Level5"};

/// Menu class that keeps track of all objects in the menu
class BaseMenu {
protected:

	std::unique_ptr<GameObject> backgroundObject;

	TextureLoader * buttonTexture;
	TextureLoader * activeButtonTexture;
	std::vector<std::unique_ptr<FontObject>> textVec;
public:
	MenuType type;
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
		textVec.clear();
	}

	void init() {
		backgroundObject = std::make_unique<GameObject>(Object_StaticImage);
		backgroundObject->addComponent<MovementHandler>(0, 0);
		backgroundObject->addComponent<TileHandler>("assets/square.png", 1);
		backgroundObject->init();
		backgroundObject->getComponent<TileHandler>()->applyColor({ 200, 0, 0 , 150});
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

		auto titleText = std::make_unique<FontObject>("assets/FreeSansBold.ttf", 80, titleRect, FontJustified_CENTER);

		switch (type) {
		case M_Main:
			addButton(BID_PlayMode);
			addButton(BID_High_Scores);
			addButton(BID_Options);
			addButton(BID_Quit);
			titleText->setText("Bubble Trouble", BLACK);

			break;
		case M_PlayMode:
			addButton(BID_Level);
			addButton(BID_Infinite);
			addButton(BID_Back);
			titleText->setText(buttonText[BID_PlayMode], BLACK);

			break;
		case M_Level:
			titleText->setText("Level Select", BLACK);
			addButton(BID_Level1);
			addButton(BID_Level2);
			addButton(BID_Level3);
			addButton(BID_Level4);
			addButton(BID_Level5);
			addButton(BID_Back);
			break;
		case M_Infinite:
			addButton(BID_1Player);
			addButton(BID_2Player);
			addButton(BID_Back);
			titleText->setText(buttonText[BID_Infinite], BLACK);
			break;
		case M_Options:
			addButton(BID_Volume);
			addButton(BID_Back);
			titleText->setText(buttonText[BID_Options], BLACK);
			break;
		case M_Volume:
			addButton(BID_Min);
			addButton(BID_Med);
			addButton(BID_Max);
			addButton(BID_Mute);
			addButton(BID_Back);
			titleText->setText(buttonText[BID_Volume], BLACK);
			break;
		case M_HighScore:
			titleText->setText("High Score", BLACK);
			addButton(BID_Back, 550);
			break;
		case M_Paused:
			addButton(BID_Continue);
			addButton(BID_PlayMode);
			addButton(BID_High_Scores);
			addButton(BID_Options);
			addButton(BID_Quit);
			titleText->setText("Paused", BLACK);
			break;

		default:
			break;
		}

		textVec.emplace_back(std::move(titleText));
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

	void addButton(ButtonID ID, int posY) {
		GameObject * button;
		FontObject * fontobject;

		button = new GameObject(Object_StaticImage);
		button->addComponent<TileHandler>(buttonTexture, 0.8);
		button->addComponent<MovementHandler>(0, 0);

		button->init();
		button->getComponent<MovementHandler>()->setPosition(double(backgroundObject->render_rect.w / 2 - button->render_rect.w / 2),
			(double)(posY));
		SDL_Rect rect = button->render_rect;
		fontobject = new FontObject(font, rect, FontJustified_CENTER);
		fontobject->setText(buttonText[ID], BLACK);
		std::unique_ptr<GameObject> uButton{ button };
		std::unique_ptr<FontObject> uFont{ fontobject };

		buttons[ID] = std::make_pair(std::move(uButton), std::move(uFont));
		buttonIDs.push_back(ID);
	}


	/// Set the next button as the active button
	void nextButton() {
		if (activeButton < buttonIDs.size() - 1) {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton + 1);
		}
		else {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(0);
		}
	}

	/// Set the previous button as the active button
	void previousButton() {
		
		if (activeButton > 0) {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(activeButton - 1);
		}
		else {
			buttons[buttonIDs[activeButton]].first->getComponent<TileHandler>()->setTextureLoader(buttonTexture);
			setActiveButton(buttonIDs.size()-1);
		}
	}

	/// Set a soecufuc button as the active button
	void setActiveButton(Uint64 number) {
		buttons[buttonIDs[number]].first->getComponent<TileHandler>()->setTextureLoader(activeButtonTexture);
		activeButton = number;
	}

	virtual void draw() {
		backgroundObject->draw();
		for (auto& a : buttonIDs) {
			buttons[a].first->draw();
			buttons[a].second->draw();
		}
		for (auto & text : textVec) {
			text->draw();
		}
	}
};

class ScoreMenu : public BaseMenu {
protected:
	std::vector<std::pair<std::unique_ptr<FontObject>, std::unique_ptr<FontObject>>> scoreList;

public:

	ScoreMenu(MenuType type, TextureLoader * button, TextureLoader * activeButton) : BaseMenu(type, button, activeButton) {
		makeScoreBoard();
	}
	~ScoreMenu() {
		scoreList.clear();
		buttons.clear();
		textVec.clear();
	}
	struct greater {
		template<class T>
		bool operator()(T const &a, T const &b) const { return a > b; }
	};

	/// Create the high score 
	void makeScoreBoard() {

		std::cout << "score\n";

		std::vector <int> fScore;
		std::string line;
		std::ifstream myfile("scoreboard.txt");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				fScore.push_back(std::stoi(line));
			}
			myfile.close();

			std::sort(fScore.begin(), fScore.end(), greater());
			//for (auto &fScore_print : fScore) {
			//	std::cout << "score = " << fScore_print << std::endl;
			//}
		}
		SDL_Rect arect = { backgroundObject->render_rect.w/2 - 100, backgroundObject->render_rect.h / 2 - 50, 200, 24 };

		for (int j = 0; j < 8 && j < fScore.size(); j++) {
			
			auto decText = new FontObject(font, arect, FontJustified_LEFT);
			decText->setText(std::to_string(j+1), BLACK);
			auto scoreText = new FontObject(font, arect, FontJustified_RIGHT);
			scoreText->setText(std::to_string(fScore.at(j)), BLACK);
			std::unique_ptr<FontObject> uScore{ scoreText };
			std::unique_ptr<FontObject> uDec{ decText };
			arect.y += 30;
			scoreList.emplace_back(std::make_pair(std::move(uDec), std::move(uScore)));
		}
	}

	void draw() override {
		BaseMenu::draw();
		for (auto& a : scoreList) {
			a.first->draw();
			a.second->draw();
		}
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
	~MenuManager() {
		menu.clear();
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
		if (type == M_HighScore) {
			ScoreMenu * pushedmenu = new ScoreMenu(type, buttonTexture.get(), activeButtonTexture.get());
			menu.emplace_back(std::move(pushedmenu));
		}
		else {
			BaseMenu * pushedmenu = new BaseMenu(type, buttonTexture.get(), activeButtonTexture.get());
			menu.emplace_back(std::move(pushedmenu));
		}
		
	}

	/// Pop a menu from the stack.
	void popMenu() {
		if(!menu.empty()) menu.pop_back();
	}

	/// Clear all the menus
	void clearMenu() {
		menu.clear();
	}

};