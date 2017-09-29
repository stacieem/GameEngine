//
//  PhysicsProperties
//
//  Created by Trystan Jasperson.
//
//
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

/*
	Holds all the properties that are used for any new
	object.
*/
class PhysicsProperties
{
public:
	PhysicsProperties(b2World& world) 
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.0f);

		body = world.CreateBody(&bodyDef);
		dynamicBox.SetAsBox(0.5f, 0.5f);
		dynamicCircle.m_radius = .5;
		dynamicCircle.m_p.Set(0, 0);
		fixtureDef.shape = &dynamicBox;

		//default properties
		fixtureDef.friction = 0.7f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.density = 1.0f;

		this->myFixture = body->CreateFixture(&fixtureDef);
	}
	
	PhysicsProperties(b2World& world, float originx, float originy,float width,float height) 
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(originx, originy);

		body = world.CreateBody(&bodyDef);
		dynamicBox.SetAsBox(width/2, height/2);

		fixtureDef.shape = &dynamicBox;

		//default properties
		fixtureDef.friction = 0.2f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.density = 1.0f;

		this->myFixture = body->CreateFixture(&fixtureDef);
	}
	
	~PhysicsProperties()
	{
    
	}
	/// body accessors and modifiers

	/**************************************************************************
	*
	*	Set the linear Velocity of the body along the x
	*
	**************************************************************************/
	void setLinearVelocity(GLfloat x, GLfloat y)
	{
		body->SetLinearVelocity(b2Vec2(x,y));
	}
	/**************************************************************************
	*
	*	Set the linear Damping of the body by damping amount
	*	a value used to represent the value reduce the linear velocity on
	*	each step()
	**************************************************************************/
	void setLinearDamping(GLfloat damping)
	{
		body->SetLinearDamping(damping);
	}

	/**************************************************************************
	*
	*	Set the angular velocity
	*
	**************************************************************************/
	void setAngularVelocity(GLfloat angVel)
	{
		body->SetAngularVelocity(angVel);
	}
	/**************************************************************************
	*
	*	Set the angular Damping of the body by damping amount
	*
	**************************************************************************/
	void setAngularDamping(GLfloat damping)
	{
		body->SetAngularDamping(damping);
	}

	/**************************************************************************
	*
	*	Set the torque on the body
	*
	**************************************************************************/
	void setTorque(GLfloat torque)
	{
		body->ApplyTorque(torque);
	}
	/**************************************************************************
	*
	*	Check for any collisions with other objects
	*
	**************************************************************************/
	void checkCollisionsWithOtherObjects()
	{
		for (b2ContactEdge* otherBody = body->GetContactList(); otherBody; otherBody = otherBody->next)
		{
			if (otherBody->contact->IsTouching())
			{
				/// we have a collision with this object
				/// additional information
				///http://www.iforce2d.net/b2dtut/collision-anatomy
			}
		}
	}

	/**************************************************************************
	*
	*	moves the center of the object over by x and y units
	*	Note: Conversion equation would be nice from pixels/coords to position
	*
	**************************************************************************/
	void translateBy(GLfloat x, GLfloat y)
	{
		b2Vec2 center = body->GetPosition();
		center.x += x;
		center.y += y;
		body->SetTransform(center, 0.0);
	}
	/**************************************************************************
	*
	*	moves the center of the object over by x and y units
	*	Note: Conversion equation would be nice from pixels/coords to position
	*
	**************************************************************************/
	void translateTo(GLfloat x, GLfloat y)
	{
		body->SetTransform(b2Vec2(x,y), 0.0);
	}
	/**************************************************************************
	*
	*	rotates the body around the current center a certain number of degrees
	*
	**************************************************************************/
	void rotate(float deg)
	{
		body->SetTransform(body->GetPosition(), (deg * DEGTORAD));
	}

	/**************************************************************************
	*
	*	get the current angle of the shape
	*
	**************************************************************************/
	float getRotation()
	{
		return body->GetAngle() * RADTODEG;
	}

	/**************************************************************************
	*
	*	get the current origin position of the shape
	*
	**************************************************************************/
	b2Vec2 GetPosition()
	{
		return body->GetPosition();
	}

	/// FixtureDef Properties

	/**************************************************************************
	*
	*	set the amount of energy retained after a collision
	*	dubbed 'bounciness' by the wise-crew
	*
	**************************************************************************/
	void setRestitution(float32 rest)
	{b2Fixture* f = body->GetFixtureList();
		f->SetRestitution(rest);
	}
	/**************************************************************************
	*
	*	set the friction for the object
	*
	**************************************************************************/
	void setFriction(float32 fric)
	{
		b2Fixture* f = body->GetFixtureList();
		f->SetFriction(fric);
	}
	/**************************************************************************
	*
	*	set the density of the body, reset body to apply correct mass
	*
	**************************************************************************/
	void setDensity(float32 dens)
	{
		b2Fixture* f = body->GetFixtureList();
		f->SetDensity(dens);
		body->ResetMassData();
	}
	/**************************************************************************
	*
	*	set a gradual Force to act on the body with respect to the x, y axis
	*	
	*
	**************************************************************************/
	void setForce(GLfloat x, GLfloat y)
	{
		body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter());
	}
	b2Vec2 getLinearVel() 
	{
		return body->GetLinearVelocity();
	}
	/**************************************************************************
	*
	*	set an immediate Force to act on the body with respect to the x, y axis
	*
	*
	**************************************************************************/
	void setImpulse(GLfloat x, GLfloat y)
	{
		body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter());
	}

	/// b2Shape properties
	void resizeCollisionBox(GLfloat x, GLfloat y)
	{
		dynamicBox.SetAsBox(x, y);
		fixtureDef.shape = &dynamicBox;
		body->DestroyFixture(this->myFixture);

		this->myFixture = body->CreateFixture(&fixtureDef);
	}
    
    /** Determines whether or not new Collisions are present in an object.
        Returns true if new collisions have been added, returns false if not.
     */
    bool hasNewCollisions()
    {
        bool hasNewCollisions = false;
        int curNumCollisions = 0;
        for (b2ContactEdge* edge = body->GetContactList(); edge; edge = edge->next)
        {
            curNumCollisions += edge->contact->GetManifold()->pointCount;
        }
        
        if (curNumCollisions > lastNumCollsions)
            hasNewCollisions = true;
        
        lastNumCollsions = curNumCollisions;
        
        return hasNewCollisions;
    }
    
    
private:
	const float RADTODEG = 57.29577951308;
	const float DEGTORAD = 0.017453292519;
	const float PI = 3.14159;
	b2Body* body;
    
    /** The number of collisions that were last checked. */
    int lastNumCollsions = 0;
    
    /** Any new collisions have been reported. */
    bool reportedNewCollsions = false;

    
	b2Fixture* myFixture;
	b2FixtureDef fixtureDef;	
	b2BodyDef bodyDef;
	b2PolygonShape dynamicBox;
	b2CircleShape dynamicCircle;

};
