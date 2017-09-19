#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class PlayerObject : public GameObject{
public:
	PlayerObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		xVel = 2.0f;
		yVel = 6.0f;
		
		getPhysicsProperties().setFriction(0.6f);
		linearDamp = 0.5f;
		origin = getPhysicsProperties().GetPosition();
	}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y += yVel;

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y -= yVel;

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLeft()
	{

		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x -= xVel;

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x += xVel;

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void reset()
	{
		translateTo(origin.x, origin.y);
		getPhysicsProperties().setLinearVelocity(0.0f,0.0f);
	}
private:
	GLfloat xVel, yVel;
	GLfloat linearDamp;
	b2Vec2 origin;

	Vector3D<GLfloat> position;
	OwnedArray<Vector3D<GLfloat>> vertices; // The vertices from the origin
	ScopedPointer<GLfloat> glVertices;

};