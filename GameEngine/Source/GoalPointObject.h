#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"

class GoalPointObject : public GameObject {
public:
	GoalPointObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		setName("Goal");
		origin = b2Vec2(0, 0);
		radius = 1.5;
		objType = GameObjectType::Checkpoint;
        getPhysicsProperties().setIsStatic(true);
		LevelToGoTo = 1;
		goToWin = false;
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
		return LevelToGoTo;
	}
	void setLevelToGoTo(int level) {
		LevelToGoTo = level;
	}
	
	// Whether or not this checkpoint lets you win
	bool getToWin() {
		return goToWin;
	}
	void setToWin() {
		goToWin = !goToWin;
	}

private:
	b2Vec2 origin;
	int LevelToGoTo;
	bool goToWin;
	float radius;

	JUCE_LEAK_DETECTOR(GoalPointObject)
};
