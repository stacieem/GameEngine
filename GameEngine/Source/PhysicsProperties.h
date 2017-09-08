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

	b2Body* getBody()
	{
		return body;
	}
private:
	b2Body* body;
};
