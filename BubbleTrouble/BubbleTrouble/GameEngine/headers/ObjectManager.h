#pragma once
#include "GameObject.h"


const std::size_t maxObjectTypes = MAX_OBJECTS;

class ObjectManager {

	std::array<std::vector<std::unique_ptr<GameObject>>, maxObjectTypes> objectGroups;

	std::size_t ObjectIDs = 0;

	/// Purpose is to keep a unique ID to the template Object of type T
	template <typename T> std::size_t getObjectID() {
		// Since ID is static, it is stored at run-time, so each different typename T has a different ID
		static std::size_t ID = ObjectIDs++;
		return ID;
	}

public:

	ObjectManager() {}
	
	template <typename T, typename... Ts> T* addObject(Ts&&... args) {
		auto& object = objectGroups[getObjectID<T>()].emplace_back(std::make_unique<T>(std::forward<Ts>(args)...));
		return object.get();
	}

	template <typename T> std::vector<std::unique_ptr<GameObject>* getObjectType() {
		return objectGroups[getObjectID<T>()];
	}

	void update() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				if(object->isValid()) object->update();
			}
		}
	}

	void draw() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				if (object->isValid()) object->draw();
			}
		}
	}

};