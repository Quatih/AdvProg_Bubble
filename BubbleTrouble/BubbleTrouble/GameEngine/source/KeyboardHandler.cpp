
#include <iostream>
#include "../headers/KeyboardHandler.h"
#include "../headers/GameObject.h"
//void KeyboardHandler::init()
//{
//	std::cout << "Initialising keyboard handler\n";
//}



//void KeyboardHandler::update()
//{
//	std::cout << "You are in update \n";
//	SDL_Event keyPress;
//
//	if (SDL_PollEvent(&keyPress) == 0)
//	{
//		;
//	}
//	else if (keyPress.type == SDL_KEYDOWN)
//	{
//
//		switch (keyPress.key.keysym.sym)
//		{
//		case SDLK_UP:
//			break;
//
//		case SDLK_LEFT:
//			owner->getComponent<MovementHandler>()->velocity->x = -1;
//			owner->getComponent<MovementHandler>()->velocity->y = 0;
//			std::cout << "I am going left" << std::endl;
//			break;
//
//		case SDLK_RIGHT:
//			owner->getComponent<MovementHandler>()->velocity->x = 1;
//			std::cout << "I am going right" << std::endl;
//			break;
//
//		case SDLK_SPACE:
//			std::cout << "I am going to kill you" << std::endl;
//			//Do something
//			break;
//
//		default:
//			break;
//
//
//
//		}
//	}
//}
