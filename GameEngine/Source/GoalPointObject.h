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
		updateState(GameObject::STATIC);
		setBodyInfo();
	}
	bool collision(PlayerObject& player) {
		bool triggered = false;
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			if (leng < radius) {
				// go to next level
				triggered = true;
			}
		
		return triggered;
	}
private:
	b2Vec2 origin;
	float radius;
};