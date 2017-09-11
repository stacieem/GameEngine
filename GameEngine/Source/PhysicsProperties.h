#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

/*
	Holds all the properties that are used for any new
	object.
*/
class PhysicsProperties
{
public:
	b2FixtureDef fixtureDef;	//provide access to internal variables
	b2BodyDef bodyDef;
	b2PolygonShape dynamicBox;

	PhysicsProperties(b2World& world) 
	{
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 0.0f);

		body = world.CreateBody(&bodyDef);
		dynamicBox.SetAsBox(0.5f, 0.5f);

		fixtureDef.shape = &dynamicBox;

		//default properties
		fixtureDef.friction = 0.2f;
		fixtureDef.restitution = 0.0f;
		fixtureDef.density = 1.0f;

		body->CreateFixture(&fixtureDef);
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

		body->CreateFixture(&fixtureDef);
	}
	
	~PhysicsProperties()
	{

	}
	/// body accessors and modifiers
	b2Body* getBody()
	{
		return body;
	}

	void translate(GLfloat x, GLfloat y)
	{
		body->SetTransform(b2Vec2(x, y), 0.0);
	}
	void rotate(float deg)
	{
		body->SetTransform(body->GetPosition(), (deg * PI) /180);
	}
	float getRotation()
	{
		return body->GetAngle();
	}
	b2Vec2 GetPosition()
	{
		return body->GetPosition();
	}

	/// FixtureDef Properties
	void setRestitution(float32 rest)
	{
		fixtureDef.restitution = rest;
	}
	void setFriction(float32 fric)
	{
		fixtureDef.friction = fric;
	}
	void setDensity(float32 dens)
	{
		fixtureDef.density = dens;
		body->ResetMassData();
	}

private:
	float radToDeg;
	float DegToRad;
	const float PI = 3.14159;
	b2Body* body;
};
