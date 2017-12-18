#pragma once
#include "GameObject.h"
#include "BubbleObject.h"

const std::size_t maxObjectTypes = MAX_OBJECTS;

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
		for (auto& group : objectGroups) {
			group.clear();
		}
	}

	template <typename T, typename... Ts> T* addObject(Ts&&... args) {
		auto& object = objectGroups[getObjectID<T>()].emplace_back(std::make_unique<T>(std::forward<Ts>(args)...));
		return dynamic_cast<T*>(object.get());
	}

	template <typename T> std::vector<T*> getObjectType() {
		std::vector<T*> vec;
		for (auto & object : objectGroups[getObjectID<T>()]){
			vec.push_back(dynamic_cast<T*>(object.get()));
		}
		return vec;	
	}
		
	void update() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				object->update();
			}
		}
	}

	void draw() {
		for (auto& group : objectGroups) {
			for (auto& object : group) {
				if(object->isVisible()) object->draw();
			}
		}
	}

	void clean() {
		for (auto object = objectGroups[getObjectID<BubbleObject>()].begin(); object != objectGroups[getObjectID<BubbleObject>()].end(); object++) {
			if ((*object) == nullptr) std::cout << "WTF" << objectGroups[getObjectID<BubbleObject>()].size() << std::endl;
			else {
				if (!(*object)->isValid()) {
					object = objectGroups[getObjectID<BubbleObject>()].erase(object);
				}
			}
		}
	}
};