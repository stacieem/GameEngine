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
		setXVelocityCap(Speed::SLOW);
		setYVelocityCap(Speed::SLOW);

		getPhysicsProperties().setFriction(0.5f);
		linearDamp = 0.5f;
		setBodyInfo();
	}
	~PlayerObject(){}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y += getYVel();
		if (store.y > getYVelocityCap()) {
			store.y = getYVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y -= getYVel();
		if (store.y < -getYVelocityCap()) {
			store.y = -getYVelocityCap();
		}

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLeft()
	{

		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x -= getXVel();
		if (store.x < -getXVelocityCap()) {
			store.x = -getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x += getXVel();
		if (store.x > getXVelocityCap()) {
			store.x = getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
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
	GLfloat linearDamp;

	Vector3D<GLfloat> position;
	OwnedArray<Vector3D<GLfloat>> vertices;	 // The vertices from the origin
	ScopedPointer<GLfloat> glVertices;

	JUCE_LEAK_DETECTOR(PlayerObject)

};
