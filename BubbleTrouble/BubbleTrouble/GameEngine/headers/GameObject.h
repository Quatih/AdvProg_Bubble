#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <memory>
#include <iostream>

#ifdef __linux__ 

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#else

#include "SDL.h"
#include "SDL_image.h"

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


const std::size_t maxComponents = 10;


enum ObjectType { Object_Player, Object_Spike, Object_Bubble };

/// Class used for each individual Game Object which has modularity with components.

class GameObject {
private:
	std::vector<GameComponent *> components;

	/// ComponentsArray used in order to be able to return a pointer to the components.
	std::array<GameComponent*, maxComponents> componentsArray;
	bool valid = true;
	std::size_t numComponents = 0;

	/// Purpose is to keep a unique ID to the template component of type T
	template <typename T> std::size_t getComponentID() {
		static std::size_t ID = numComponents++;
		return ID;
	}

public:
	int pops = 0;
	SDL_Rect img_rect, render_rect;
	ObjectType type;
	GameObject(ObjectType type) {
		this->type = type;
	}

	~GameObject() {
		components.clear();
	}

	void update() {
		if (isValid()) {
			for (auto& comps : components) {
				comps->update();
			}
		}
	}

	void init() {
		for (auto& comps : components) {
			comps->init();
		}
	}

	void draw() {
		if (isValid()) {
			for (auto& comps : components) {
				comps->draw();
			}
		}
	}

	void setValid() { valid = true; }
	void destroy() { valid = false; }

	bool isValid() { return valid; }

	/// Add component of type T with arguments Ts to this GameObject
	template <typename T, typename... Ts>
	void addComponent(Ts&&... args)	{
		// Forward arguments made to addcomponent to the newly created component
		T* comp = new T(std::forward<Ts>(args)...);
		components.push_back(comp);
		comp->owner = this;
		
		// Add the component to the array at the unique location of this template type
		componentsArray[getComponentID<T>()] = comp;
	}
	
	/// Returns true if the Object has a component of type T.
	template <typename T> bool hasComponent() {
		return (componentsArray[getComponentID<T>()] != nullptr);
	}

	/// Return pointer to the stored component of type T.
	/// Returns nullptr if the Object does not contain a component of type T.
	template <typename T> T* getComponent() {
		return static_cast<T*>(componentsArray[getComponentID<T>()]);
	}
};