#pragma once
#include "GameObject.h"

const std::size_t maxObjectTypes = MAX_OBJECTS;

/// Maintains different object types
class ObjectManager {

	std::array<std::vector<std::unique_ptr<GameObject>>, maxObjectTypes> objectGroups;
	
public:

	ObjectManager() {}

	~ObjectManager() {
		freeAll();
	}

	/// Add an object to the appropriate position, then return a pointer to it.
	GameObject* addObject(ObjectType type) {
		// Add the object to the correct vector and forward the arguments
		GameObject* ptr = (new GameObject(type));
		objectGroups[type].emplace_back(std::move(ptr));
		return ptr;
	}

	/// Add an object to the appropriate position, then return a pointer to it.
	template <typename T, typename... Ts> T* addObject(Ts&&... args) {
		// Add the object to the correct vector and forward the arguments
		T* ptr = (new T(std::forward<Ts>(args)...));
		objectGroups.at(ptr->type).emplace_back(std::move(ptr));
		return ptr;
	}

	/// Return a copy of a vector with the type T.
	/// Necessary in order to use a vector for the subtypes
	template <typename T> std::vector<T*> getObjectTypeVector(ObjectType type) {
		std::vector<T*> vec;
		for (auto & object : objectGroups.at(type)){
			vec.push_back(static_cast<T*>(object.get()));
		}
		return vec;	
	}

	/// Return a pointer to the stored vector of type T
	const std::vector<std::unique_ptr<GameObject>> * getObjectBaseVector(ObjectType type) const {
		return &(objectGroups[type]);
	}
		
	/// update each object
	void update() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				object->update();
			}
		}
	}

	/// draw each object, if they are visible in the game
	void draw() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				if(object->isVisible()) object->draw();
			}
		}
	}

	/// Removes all invalidated objects in the array.
	void clean() {
		for (auto& group : objectGroups) {
			for (auto object = group.begin(); object != group.end();) {
				if (!(*object)->isValid()) {
					object = group.erase(object);
				}
				else {
					++object;
				}

			}
		}

	}


	/// Clear each object Vector
	void freeAll() {
		for (auto& group : objectGroups) {
			group.clear();
		}
	}

};