#pragma once
#include "GameObject.h"
#include "BubbleObject.h"

const std::size_t maxObjectTypes = MAX_OBJECTS;

/// Maintains different object types
class ObjectManager {

	std::array<std::vector<std::unique_ptr<GameObject>>, maxObjectTypes> objectGroups;
	
	std::size_t uniqueID() {
		static std::size_t ObjectIDs = 0;
		return ObjectIDs++;
	}

	/// Purpose is to keep a unique ID to the template Object of type T
	template <typename T> std::size_t getObjectID() {
		// Since ID is static, it is stored at run-time, so each different typename T has a different ID
		static std::size_t ID = uniqueID();
		return ID;
	}

public:

	ObjectManager() {}

	~ObjectManager() {
		freeAll();
	}

	/// Add an object to the appropriate position, then return a pointer to it.
	template <typename T, typename... Ts> T* addObject(Ts&&... args) {
		// Add the object to the correct vector and forward the arguments
		auto ptr = std::unique_ptr<T>(new T(std::forward<Ts>(args)...));
		auto& object = objectGroups[getObjectID<T>()].emplace_back(std::move(ptr));
		return dynamic_cast<T*>(object.get());
	}

	/// Return a copy of a vector with the type T.
	/// Necessary in order to use a vector for the subtypes
	template <typename T> std::vector<T*> getObjectTypeVector() {
		std::vector<T*> vec;
		for (auto & object : objectGroups[getObjectID<T>()]){
			vec.push_back(dynamic_cast<T*>(object.get()));
		}
		return vec;	
	}

	/// Return a pointer to the stored vector of type T
	template <typename T> std::vector<std::unique_ptr<GameComponent>> * getObjectBaseVector() const {
		return &objectGroups[getObjectID<T>()];
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