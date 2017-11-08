#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class PlayerObject : public GameObject{
public:
	PlayerObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		setName("Player");
		objType = GameObjectType::Player;
		setMoveSpeed(Speed::MED);
		setJumpSpeed(Speed::SLOW);
        getPhysicsProperties().setIsStatic(false);
	}
	~PlayerObject(){}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y += getJumpSpeedVelocity();
		if (store.y > cappedJumpSpeed) {
			store.y = cappedJumpSpeed;
		}

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y -= getJumpSpeedVelocity();
		if (store.y < -cappedJumpSpeed) {
			store.y = -cappedJumpSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void moveLeft()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x -= getRunSpeedVelocity();
		if (store.x < -cappedMoveSpeed) {
			store.x = -cappedMoveSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x += getRunSpeedVelocity();
		if (store.x > cappedMoveSpeed) {
			store.x = cappedMoveSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	b2Vec2 getPosition() {
		return getPhysicsProperties().GetPosition();
	}
//	void reset()
//	{
//		translateTo(origin.x, origin.y);
//		getPhysicsProperties().setLinearVelocity(0.0f,0.0f);
//	}
private:
	Speed move;
	Speed jump;
	JUCE_LEAK_DETECTOR(PlayerObject)

};
