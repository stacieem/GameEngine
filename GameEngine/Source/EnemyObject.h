#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class EnemyObject : public GameObject {
public:
	EnemyObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		
		objType = GameObjectType::Enemy;
		aiState = CHASE;
		setXVelocityCap(Speed::Low);
		setYVelocityCap(Speed::Low);
		detection_radius = 25;
		getPhysicsProperties().setFriction(0.5f);
		linearDamp = 0.5f;
		origin = getPhysicsProperties().GetPosition();
	}
	~EnemyObject() {}
	enum AIType {
		Patrol,
		CHASE,
		SCAREDAF
	};
	void changeAI(AIType type) {
		aiState = type;
	}
	void decision(PlayerObject& player) {

		switch (aiState) {
		case Patrol:

			break;
		case CHASE:
			if ((getPosition() - player.getPosition()).Length() < 10)	//detected
			{
				b2Vec2 myPos = getPosition();
				b2Vec2 theirPos = player.getPosition();
				if (myPos.x < theirPos.x) {
					moveRight();
				}
				else if(myPos.x > theirPos.x)
				{
					moveLeft();
				}
				if (myPos.y < theirPos.y) {
					moveUp();
				}
				else if (myPos.y > theirPos.y)
				{
					moveDown();
				}
			}
			break;
		case SCAREDAF:
			if ((getPosition() - player.getPosition()).Length() < 10)	//detected
			{
				b2Vec2 myPos = getPosition();
				b2Vec2 theirPos = player.getPosition();
				if (myPos.x > theirPos.x) {
					moveRight();
				}
				else
				{
					moveLeft();
				}
				if (myPos.y > theirPos.y) {
					moveDown();
				}
				else
				{
					moveUp();
				}
			}
			break;
		}
	}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y += getYVel()/2;
		if (store.y > getYVelocityCap()) {
			store.y = getYVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y -= getYVel()/2;
		if (store.y < -getYVelocityCap()) {
			store.y = -getYVelocityCap();
		}

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLeft()
	{

		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x -= getXVel()/2;
		if (store.x < -getXVelocityCap()) {
			store.x = -getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x += getXVel()/2;
		if (store.x > getXVelocityCap()) {
			store.x = getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void reset()
	{
		translateTo(origin.x, origin.y);
		getPhysicsProperties().setLinearVelocity(0.0f, 0.0f);
	}
private:
	GLfloat linearDamp;
	b2Vec2 origin;
	
	AIType aiState;
	//bounds at which to patrol
	float leftBound, rightBound;
	float detection_radius;
	Vector3D<GLfloat> position;
	OwnedArray<Vector3D<GLfloat>> vertices;	 // The vertices from the origin
	ScopedPointer<GLfloat> glVertices;

	JUCE_LEAK_DETECTOR(EnemyObject)

};