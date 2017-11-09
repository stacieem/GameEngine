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
	}

	CollectableObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{
		getPhysicsProperties().setIsStatic(true);
		radius = 1.5;
	}
	~CollectableObject() {}

	bool collision(PlayerObject& player) {
		bool collected = false;
		if (getIsActive()) {
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			//DBG(leng);
			if (leng < radius) {
				collected = true;

				setActive(false);
				getPhysicsProperties().setActiveStatus(false);
				setRenderable(false);

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
	float radius;
	JUCE_LEAK_DETECTOR(CollectableObject)

};
