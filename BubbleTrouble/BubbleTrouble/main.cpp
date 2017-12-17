#include <iostream>
#include "GameEngine/GameEngine.h"


int main(int /*argc*/, char ** /*argv*/) {

	/*SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Bubble Trouble",
		"Have Fun.",
		NULL);*/

	const SDL_MessageBoxButtonData buttons[] = {
		{ /* .flags, .buttonid, .text */        0, 0, "no" },
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
		{ /* .colors (.r, .g, .b) */
		  /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
			{ 255,   255,   0 },
			/* [SDL_MESSAGEBOX_COLOR_TEXT] */
			{ 0, 255,   255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
			{ 255, 255,   255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
			{ 0,   0, 255 },
			/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
			{ 255,   255, 255 }
		}
	};
	 const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL, /* .window */
		"Do you want to continue the game?", /* .title */
		"select a button", /* .message */
		SDL_arraysize(buttons), /* .numbuttons */
		buttons, /* .buttons */
		&colorScheme /* .colorScheme */
	};
	int buttonid;

	

	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
		SDL_Log("error displaying message box");
		return 1;
	}

	if (buttonid == 0 || buttonid == 2)
	{
		return -1;
	}

	if (buttonid == -1) {
		SDL_Log("no selection");
	}
	else {
		SDL_Log("selection was %s", buttons[buttonid].text);
	}


	std::unique_ptr<GameEngine> Game{ new GameEngine("Bubble Trouble", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,  1024, 600, SDL_WINDOW_HIDDEN) };

	const Uint32 frameDelay = 1000 / 60;
	Uint32 frameStart;
	Uint32 frameTime;

	/// Main game loop
	while (Game->isRunning())
	{
		frameStart = SDL_GetTicks();

		//Handle events on queue
		Game->handleEvents();
		Game->update();
		Game->cleanObjects();
		Game->render();
		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime){
			SDL_Delay(frameDelay - frameTime);
		}
	}


	return 0;
}