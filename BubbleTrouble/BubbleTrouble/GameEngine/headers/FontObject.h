#pragma once
#include "GameObject.h"
#include <string>

enum FontJustified{FontJustified_LEFT, FontJustified_CENTER, FontJustified_RIGHT}; 

extern SDL_Renderer * renderer;

/// Object that renders text
class FontObject : public GameObject {
private:
public:
	SDL_Texture * message = NULL;
	TTF_Font * font;
	std::string text;
	SDL_Color color { 0,0,0,0 };
	//SDL_Color bgcolor;
	FontJustified justification;
	bool loadedFont = false;

	/// Load object with a specific font
	FontObject(std::string fontpath, int size, SDL_Rect rect, FontJustified justified) : GameObject(Object_Font) {
		render_rect = rect;
		img_rect.x = render_rect.x;
		img_rect.y = render_rect.y;
		//this->bgcolor.r = bgcolor.red;
		//this->bgcolor.b = bgcolor.blue;
		//this->bgcolor.g = bgcolor.green;
		justification = justified;
		loadFont(fontpath, size);
	}

	/// Load object with existing font reference.
	FontObject(TTF_Font * font, SDL_Rect render_rect, FontJustified justified) : GameObject(Object_Font) {
		this->render_rect = render_rect;
		img_rect.x = render_rect.x;
		img_rect.y = render_rect.y;
		//this->bgcolor.r = bgcolor.red;
		//this->bgcolor.b = bgcolor.blue;
		//this->bgcolor.g = bgcolor.green;
		justification = justified;
		this->font = font;
	}

	~FontObject() {
 		if (message != NULL) SDL_DestroyTexture(message);
		if (loadedFont) TTF_CloseFont(font);
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

		message = SDL_CreateTextureFromSurface(renderer, surface);
		if (message == NULL) {
			std::cout << "Errorloading text\n";
		}

		img_rect.w = surface->clip_rect.w;
		img_rect.h = surface->clip_rect.h;

		SDL_FreeSurface(surface);


		switch (justification) {
		case FontJustified_LEFT:
			//imgrect.y = render_rect.y + (render_rect.h - surface->clip_rect.h); // top justified
			break;
		case FontJustified_CENTER:
			img_rect.x = render_rect.x + (render_rect.w / 2 - surface->clip_rect.w / 2);
			img_rect.y = render_rect.y + (render_rect.h / 2 - surface->clip_rect.h / 2); // center justified
			break;
		case FontJustified_RIGHT:
			img_rect.x = render_rect.x + (render_rect.w - surface->clip_rect.w);
			//imgrect.y = render_rect.y + (render_rect.h - surface->clip_rect.h); // bottom justified
			break;
		default:
			break;
		}

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

	/// Load a font at the path 
	void loadFont(std::string path, int size) {
		if (loadedFont) TTF_CloseFont(font);
		font = TTF_OpenFont(path.c_str(), size);
		if (font == NULL) {
			std::cout << "Error loading font at " << path << std::endl;
		}
		loadedFont = true;
	}

	/// Set the font style and refresh texture
	void setFontStyle(int style) {
		TTF_SetFontStyle(font, style);
		setText(text, color);
	};

	/// Draw this font object
	void draw() override {
		if(visible) SDL_RenderCopyEx(renderer, message, NULL, &img_rect, 0, 0, SDL_FLIP_NONE);
	}
};
