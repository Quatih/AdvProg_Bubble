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

class GameComponent {
public:
	GameObject * owner;
	virtual ~GameComponent() = default;

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};
};

const std::size_t maxComponents = 5;

class GameObject {
private:
	std::vector<std::unique_ptr<GameComponent>> components;
	std::array<GameComponent*, maxComponents> componentsArray;
	bool valid;
	std::size_t numComponents = 0;

	/// Purpose is to keep a unique ID to the template component of type T
	template <typename T> std::size_t getComponentID() {
		static std::size_t ID = numComponents++;
		return ID;
	}

public:
	int pops;
	SDL_Rect img_rect, render_rect;

	GameObject();
	~GameObject();

	void update();
	void init();
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
		std::unique_ptr<GameComponent> unique{ comp };
		components.emplace_back(std::move(unique));
		comp->owner = this;
		componentsArray[getComponentID<T>()] = comp;
		/// Add the component to the array at the unique location of this template type
	}
	
	template <typename T> bool hasComponent() {
		return (componentsArray[getComponentID<T>()] != nullptr);
	}

	/// Return pointer to the stored component of type T
	template <typename T> T* getComponent() {
		return static_cast<T*>(componentsArray[getComponentID<T>()]);
	}
};