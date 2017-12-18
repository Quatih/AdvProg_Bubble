#pragma once
#ifdef __linux__ 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#else
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#endif

#include <iostream>
#include <string>

enum FontJustified{LEFT, CENTER, RIGHT};

class FontLoader {
private:

public:

	SDL_Renderer * renderer;
	SDL_Texture * message;
	SDL_Rect dimensions;
	SDL_Rect imgrect;
	TTF_Font * font;
	std::string path;
	SDL_Color color;
	SDL_Color bgcolor;
	FontJustified justification;
	bool visible = true;

	FontLoader(SDL_Renderer* renderer, std::string fontpath, int size, SDL_Rect dimensions, Color bgcolor, FontJustified justified) {

		this->renderer = renderer;
		this->dimensions = dimensions;
		imgrect.x = dimensions.x;
		imgrect.y = dimensions.y;
		this->bgcolor.r = bgcolor.red;
		this->bgcolor.b = bgcolor.blue;
		this->bgcolor.g = bgcolor.green;
		justification = justified;
		font = TTF_OpenFont(fontpath.c_str(), size);
		if (font == NULL) {
			std::cout << "Error loading font " << fontpath << std::endl;
		}
	}

	~FontLoader() {
		if (message != nullptr) SDL_DestroyTexture(message);
		//if(font != nullptr) TTF_CloseFont(font);
	}

	void setText(std::string str, SDL_Color color) {
		path = str;
		this->color = color;
		if (message != nullptr) SDL_DestroyTexture(message);
		SDL_Surface * surface = TTF_RenderText_Shaded(font, path.c_str(), this->color, bgcolor);
		if (surface == NULL) {
			std::cout << "Error loading text surface\n";
		}
		else {
			imgrect.w = surface->clip_rect.w;
			imgrect.h = surface->clip_rect.h;

			switch (justification) {
			case LEFT:
				break;
			case CENTER:
				imgrect.x = dimensions.x + (dimensions.w / 2 - surface->clip_rect.w / 2);
				//imgrect.y = dimensions.x + (dimensions.h/2 - surface->clip_rect.h/2);
				break;
			case RIGHT:
				imgrect.x = dimensions.x + (dimensions.w - surface->clip_rect.w);
				//imgrect.y = dimensions.y + (dimensions.h - surface->clip_rect.h);
				break;
			default:
				break;
			}


			message = SDL_CreateTextureFromSurface(renderer, surface);
			if (message == NULL) {
				std::cout << "Error loading text\n";
			}

			SDL_FreeSurface(surface);
		}
	}

	void setText(std::string str, Color color) {

		this->color.r = color.red;
		this->color.b = color.blue;
		this->color.g = color.green;
		this->color.a = 255;
		path = str;
		setText(str, this->color);
	}

	void setText(std::string str) {
		path = str;
		setText(str, color);
	}


	void setColor(Color color) {
		setText(path, color);
	}


	void hide() {
		visible = false;
	}

	void show() {
		visible = true;
	}
	void setFontStyle(int style) {
		TTF_SetFontStyle(font, style);
		setText(path, color);
	};

	void draw() {
		if(visible) SDL_RenderCopyEx(renderer, message, NULL, &imgrect, 0, 0, SDL_FLIP_NONE);
	}
};
