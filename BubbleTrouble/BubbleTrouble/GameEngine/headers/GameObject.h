#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <memory>
#include <iostream>
#include "SDL.h"

class GameObject;

class GameComponent {
public:
	GameObject * owner;
	virtual ~GameComponent() = default;

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
};

const std::size_t maxComponents = 10;

class GameObject {
private:
	std::vector<GameComponent*> components;
	std::array<GameComponent*, maxComponents> componentArray;
	bool valid;

	std::size_t getUniqueID() {
		/// Maintains value during runtime
		static std::size_t ID = 0;
		return ID++;
	}

	/// Purpose is to keep a unique ID to the template component of type T
	template <typename T> std::size_t getComponentID() {
		static std::size_t ID = getUniqueID();
		return ID;
	}

public:
	int pops;
	SDL_Rect default_rect, render_rect;

	GameObject(int width, int height, float scale);
	~GameObject();

	void update();
	void setValid() { valid = true; }
	bool isValid() { return valid; };
	void destroy() { valid = false; };
	void draw();
	/// Add component of type T with arguments Ts to this GameObject
	template <typename T, typename... Ts>
	void addComponent(Ts&&... args)
	{
		/// Forward arguments made to addcomponent to the newly created component
		T* comp = new T(std::forward<Ts>(args)...);
		comp->owner = this;
		/// Add the component to the array at the unique location of this template type
		components.push_back(comp);
		componentArray[getComponentID<T>()] = comp;

		comp->init();
	}
	
	template <typename T> bool hasComponent() {
		return (componentArray[getComponentID<T>()] != nullptr);
	}

	/// Return pointer to the stored component of type T
	template <typename T> T* getComponent() {
		return static_cast<T*>(componentArray[getComponentID<T>()]);
	}
};