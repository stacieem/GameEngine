#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class PlayerObject : public GameObject{
public:
	PlayerObject(WorldPhysics & worldPhysics, Model* model) : GameObject(worldPhysics, model)
	{
		setName("Player");
		objType = GameObjectType::Player;
		setMoveSpeed(Speed::MED);
		setJumpSpeed(Speed::SLOW);
        getPhysicsProperties().setIsStatic(false);
	}

	PlayerObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{
		/*objType = GameObjectType::Player;
		setXVelocityCap(Speed::SLOW);
		setYVelocityCap(Speed::SLOW);
		*/
		getPhysicsProperties().setFriction(0.5f);

		//origin = getPhysicsProperties().GetPosition();
		parseFrom(valueTree);
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


	ValueTree serializeToValueTree() {

		ValueTree playerSerialization = GameObject::serializeToValueTree();



	

		return playerSerialization;
	}
    

	b2Vec2 getPosition() {
		return getPhysicsProperties().GetPosition();
	}


private:

	GLfloat linearDamp;
	Speed move;
	Speed jump;

	JUCE_LEAK_DETECTOR(PlayerObject)

};
