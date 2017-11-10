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

		levelToGoTo = 1;
		goToWin = false;
	}

	GoalPointObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{
		parseFrom(valueTree);
		radius = 1.5;
		levelToGoTo = 0;
	}

	/** Copy Constructor - Used to easily make a copy of an existing GameObject
	(this is directly used by the WorldNavigator when alt-dragging)
	*/
	GoalPointObject(GoalPointObject & objectToCopy, WorldPhysics & worldPhysics) : GameObject(objectToCopy, worldPhysics)
	{
		radius = objectToCopy.radius;
		levelToGoTo = objectToCopy.levelToGoTo;
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

		ValueTree goToWinTree("GoToWin");

		goToWinTree.setProperty(Identifier("value"), var(goToWin), nullptr);

		goalPointSerialization.addChild(goToWinTree, -1, nullptr);

		return goalPointSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		GameObject::parseFrom(valueTree);

		ValueTree levelToGoToValueTree = valueTree.getChildWithName(Identifier("LevelToGoTo"));

		levelToGoTo = levelToGoToValueTree.getProperty(Identifier("value"));

		ValueTree goToWinTree = valueTree.getChildWithName(Identifier("GoToWin"));

		goToWin = goToWinTree.getProperty(Identifier("value"));

	}

	// Whether or not this checkpoint lets you win
	bool getToWin() {
		return goToWin;
	}
	void setToWin() {
		goToWin = !goToWin;
	}

private:
	int levelToGoTo;

	b2Vec2 origin;
	bool goToWin;

	float radius;

	JUCE_LEAK_DETECTOR(GoalPointObject)
};
