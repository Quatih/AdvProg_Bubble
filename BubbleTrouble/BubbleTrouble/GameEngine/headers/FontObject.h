#pragma once
#include "GameObject.h"
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

extern SDL_Renderer * renderer;

/// Object that renders text
class FontObject : public GameObject {
private:

public:
	SDL_Texture * message = NULL;
	SDL_Rect dimensions;
	SDL_Rect imgrect;
	TTF_Font * font;
	std::string text;
	SDL_Color color;
	SDL_Color bgcolor;
	FontJustified justification;

	FontObject(std::string fontpath, int size, SDL_Rect dimensions, Color bgcolor, FontJustified justified) : GameObject(Object_Font) {

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

	FontObject(TTF_Font * font, SDL_Rect dimensions, Color bgcolor, FontJustified justified) : GameObject(Object_Font) {
		this->dimensions = dimensions;
		imgrect.x = dimensions.x;
		imgrect.y = dimensions.y;
		this->bgcolor.r = bgcolor.red;
		this->bgcolor.b = bgcolor.blue;
		this->bgcolor.g = bgcolor.green;
		justification = justified;
		this->font = font;
	}

	~FontObject() {
 		if (message != NULL) SDL_DestroyTexture(message);
		//if(font != nullptr) TTF_CloseFont(font);
	}
	/// creates a new texture with the passed text
	void setText(std::string str, SDL_Color color) {
		text = str;
		this->color = color;
		if (message != NULL) SDL_DestroyTexture(message);
		//SDL_Surface * surface = TTF_RenderText_Shaded(font, text.c_str(), this->color, bgcolor);
		SDL_Surface * surface = TTF_RenderText_Blended(font, text.c_str(), this->color);
		if (surface == NULL) {
			std::cout << "Error loading text surface\n";
		}

		imgrect.w = surface->clip_rect.w;
		imgrect.h = surface->clip_rect.h;

		switch (justification) {
		case LEFT:
			//imgrect.y = dimensions.y + (dimensions.h - surface->clip_rect.h); // top justified
			break;
		case CENTER:
			imgrect.x = dimensions.x + (dimensions.w / 2 - surface->clip_rect.w / 2);
			imgrect.y = dimensions.y + (dimensions.h/2 - surface->clip_rect.h/2); // center justified
			break;
		case RIGHT:
			imgrect.x = dimensions.x + (dimensions.w - surface->clip_rect.w);
			//imgrect.y = dimensions.y + (dimensions.h - surface->clip_rect.h); // bottom justified
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
	

	/// Set the text with a COlor struct
	void setText(std::string str, Color color) {

		this->color.r = color.red;
		this->color.b = color.blue;
		this->color.g = color.green;
		this->color.a = 255;
		text = str;
		setText(text, this->color);
	}

	/// Set the text with an already provided color
	void setText(std::string str) {
		text = str;
		setText(text, color);
	}

	/// Set the color of the already provided text
	void setColor(Color color) {
		setText(text, color);
	}

	/// Set the font style and refresh texture
	void setFontStyle(int style) {
		TTF_SetFontStyle(font, style);
		setText(text, color);
	};

	/// Draw
	void draw() override {
		if(visible) SDL_RenderCopyEx(renderer, message, NULL, &imgrect, 0, 0, SDL_FLIP_NONE);
	}
};
