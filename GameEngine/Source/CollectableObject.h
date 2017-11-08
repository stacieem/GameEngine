#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "Level.h"
/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class CollectableObject : public GameObject {
public:
	CollectableObject(WorldPhysics & worldPhysics, Model* model) : GameObject(worldPhysics, model)
	{
		objType = GameObjectType::Collectable;
		setName("Collectable");
        getPhysicsProperties().setIsStatic(true);
		radius = 1.5;
		isActive = true;
	}

	CollectableObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{

		radius = 1.5;
		isActive = true;
	}
	~CollectableObject() {}

	bool collision(PlayerObject& player) {
		bool collected = false;
		if (isActive) {
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			//DBG(leng);
			if (leng < radius) {
				// go to next level
				collected = true;
				isActive = false;
				getPhysicsProperties().removeCollisionBox();
				//make this disappear so that we don't mess with it anymore
				// make it invisible
			}
		}
		return collected;
	}

	ValueTree serializeToValueTree() {

		ValueTree collectableSerialization = GameObject::serializeToValueTree();

		return collectableSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		GameObject::parseFrom(valueTree);

	}

private:
	bool isActive;
	float radius;
	JUCE_LEAK_DETECTOR(CollectableObject)

};
