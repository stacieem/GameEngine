#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"

class GoalPointObject : public GameObject {
public:
	GoalPointObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		origin = b2Vec2(0, 0);
		radius = .5;
	}

	bool collision(PlayerObject& player) {
		bool triggered = false;
		b2Vec2 otherOrigin = player.getPosition();

		if ((otherOrigin - origin).Length < radius) {
			// go to next level
			triggered = true;
		}

		return triggered;
	}

private:
	b2Vec2 origin;
	float radius;
};