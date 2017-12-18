#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <memory>
#include <iostream>

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#else

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#endif

class GameObject;

/// Base component class for inheritance.
class GameComponent {
public:
	GameObject * owner;
	virtual ~GameComponent() = default;

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
};

/// Hold the different types of objects
enum ObjectType : std::size_t { Object_Player, Object_Spike, Object_Bubble, Object_Explosion, Object_Lives, Object_Font, MAX_OBJECTS};

const std::size_t maxComponents = 10;

/// Class used for each individual Game Object which has modularity with components.
class GameObject {
protected:
	std::vector<std::unique_ptr<GameComponent>> components;

	/// ComponentsArray used in order to be able to return a pointer to the components.
	std::array<GameComponent*, maxComponents> componentsArray;
	bool valid = true;
	bool visible = true;
	std::size_t componentIDs = 0;

	/// Purpose is to keep a unique ID to the template component of type T
	template <typename T> std::size_t getComponentID() {
		// Since ID is static, it is stored at run-time, so each different typename T has a different ID
		static std::size_t ID = componentIDs++;
		return ID;
	}

public:
	SDL_Rect img_rect, render_rect;
	ObjectType type;

	GameObject(ObjectType type) {
		this->type = type;
	}

	virtual ~GameObject() {
		components.clear();
	}

	virtual void update() {
		if (isValid()) {
			for (auto& comps : components) {
				comps->update();
			}
		}
	}

	virtual void init() {
		for (auto& comps : components) {
			comps->init();
		}
	}

	virtual void draw() {
		if (isValid()) {
			for (auto& comps : components) {
				comps->draw();
			}
		}
	}


	void hide() { visible = false; }
	void show() { visible = true;  }
	bool isVisible() { return visible; }
	void setValid() { valid = true; }
	void destroy() { valid = false; }
	bool isValid() const { return valid; }

	/// Add component of type T with arguments Ts to this GameObject
	template <typename T, typename... Ts>
	void addComponent(Ts&&... args) {
		/// Forward arguments made to addcomponent to the newly created component
		auto& comp = components.emplace_back(std::make_unique<T>(std::forward<Ts>(args)...));
		comp->owner = this;

		/// Add the component to the array at the unique location of this template type
		componentsArray[getComponentID<T>()] = comp.get();
	}

	/// Returns true if the Object has a component of type T.
	template <typename T> bool hasComponent() {
		return (componentsArray[getComponentID<T>()] != nullptr);
	}

	/// Return pointer to the stored component of type T.
	/// Returns nullptr if the Object does not contain a component of type T.
	template <typename T> T* getComponent() {
		if(hasComponent<T>()) return static_cast<T*>(componentsArray[getComponentID<T>()]);
		else throw std::exception("Requested Component is non-existent");
	}
};