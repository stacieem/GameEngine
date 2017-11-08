#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"

class GoalPointObject : public GameObject {
public:
	GoalPointObject(WorldPhysics & worldPhysics, Model* model) : GameObject(worldPhysics, model)
	{
		setName("Goal");
		radius = 1.5;
		objType = GameObjectType::Checkpoint;
        getPhysicsProperties().setIsStatic(true);
		levelToGoTo = 0;
	}

	GoalPointObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{
		parseFrom(valueTree);
		radius = 1.5;
		levelToGoTo = 0;
	}

	bool collision(PlayerObject& player) {
		bool triggered = false;
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			if (leng < radius) {
				triggered = true;
			}
		
		return triggered;
	}
	
	// Destination level
	int getLevelToGoTo() {
		return levelToGoTo;
	}
	void setLevelToGoTo(int level) {
		levelToGoTo = level;
	}

	ValueTree serializeToValueTree() {

		ValueTree goalPointSerialization = GameObject::serializeToValueTree();

		ValueTree levelToGoToValueTree("LevelToGoTo");

		levelToGoToValueTree.setProperty(Identifier("value"), var(levelToGoTo), nullptr);

		goalPointSerialization.addChild(levelToGoToValueTree, -1, nullptr);

		return goalPointSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		GameObject::parseFrom(valueTree);

		ValueTree levelToGoToValueTree = valueTree.getChildWithName(Identifier("LevelToGoTo"));

		levelToGoTo = levelToGoToValueTree.getProperty(Identifier("value"));

	}

private:
	int levelToGoTo;
	float radius;

	JUCE_LEAK_DETECTOR(GoalPointObject)
};
