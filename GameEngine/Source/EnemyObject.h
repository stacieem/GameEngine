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
		aiState = NONE;
		setXVelocityCap(Speed::SLOW);
		setYVelocityCap(Speed::MED);
		detection_radius = 7;
		getPhysicsProperties().setFriction(0.5f);
		//linearDamp = 0.5f;
		setBodyInfo();
		setName("Enemy");
		direction = 1;
		timeToSwap = 200;	//value that deltaTime will sum to for us to determine we need to switch directions
		timeElapsed = 0;
        
        getPhysicsProperties().setIsStatic(false);
	}

	~EnemyObject() {}

	enum AIType {
		NONE,
		GROUNDPATROL,
		JUMPPATROL,
		SCAREDAF,
		CHASE
	};
	void changeAI(AIType type) {
		aiState = type;
	}

	AIType getAIState() {
		return aiState;
	}

	void decision(PlayerObject& player, double elapsed) {
		switch (aiState) {
		case GROUNDPATROL:
			if (timeElapsed >= timeToSwap) {
				timeElapsed = 0;
				direction *= -1;
				b2Vec2 store = getPhysicsProperties().getLinearVel();
				getPhysicsProperties().setLinearVelocity(0, store.y);
			}
			else
			{
				timeElapsed += 1;
			}
			moveLateral();
			break;
		case JUMPPATROL:
			if (timeElapsed >= timeToSwap) {
				timeElapsed = 0;
				direction *= -1;
				b2Vec2 store = getPhysicsProperties().getLinearVel();
				getPhysicsProperties().setLinearVelocity(0, store.y);
			}
			else
			{
				timeElapsed += 1;
			}
			jumpLateral();
			break;
		case CHASE:
			if ((b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y) - 
					b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y)).Length() < detection_radius)	//detected
			{
				b2Vec2 myPos = b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y);
				b2Vec2 theirPos = b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y);
				if (myPos.x < theirPos.x) {
					moveRight();
				}
				else if (myPos.x > theirPos.x)
				{
					moveLeft();
				}
				if (myPos.y < theirPos.y) {
					//moveUp();
				}
				else if (myPos.y > theirPos.y)
				{
					//moveDown();
				}
			}
			break;
		case SCAREDAF:
			if ((b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y) - 
					b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y)).Length() < detection_radius)	//detected
			{
				b2Vec2 myPos = b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y);
				b2Vec2 theirPos = b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y);
				if (myPos.x > theirPos.x) {
					moveRight();
				}
				else if (myPos.x < theirPos.x)
				{
					moveLeft();
				}
				if (myPos.y > theirPos.y) {
					//moveUp();
				}
				else if (myPos.y < theirPos.y)
				{
					//moveDown();
				}
			}
			break;
		}
	}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y = -getYVel() / 2;
		if (store.y > getYVelocityCap()) {
			store.y = getYVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y = getYVel() / 2;
		if (store.y < -getYVelocityCap()) {
			store.y = -getYVelocityCap();
		}

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLeft()
	{

		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = -getXVel();
		if (store.x < -getXVelocityCap()) {
			store.x = -getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getXVel();
		if (store.x > getXVelocityCap()) {
			store.x = getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLateral() {
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getXVel() * direction;
		if (store.x > getXVelocityCap()) {
			store.x = getXVelocityCap();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void jumpLateral() {
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getXVel() * direction;
		if (store.x > getXVelocityCap()) {
			store.x = getXVelocityCap();
		}
		if (store.y == 0) {
			store.y = getYVel();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
private:
	float linearDamp;
	double timeToSwap,timeElapsed;
	int direction;
	AIType aiState;
	//bounds at which to patrol

	float detection_radius;
	Vector3D<GLfloat> position;
	OwnedArray<Vector3D<GLfloat>> vertices;	 // The vertices from the origin
	ScopedPointer<GLfloat> glVertices;


	JUCE_LEAK_DETECTOR(EnemyObject)

};
