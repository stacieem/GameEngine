//
//  WorldPhysics
//
//  Created by Trystan Jasperson.
//
//
#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

class WorldPhysics {
	
public:
	WorldPhysics() : gravity(0.0f, -10.0f), world(gravity)
	{
		velocityIterations = 2.0f;
		positionIterations = 6.0f;
		timeStep = 1.0f / 15.0f;
        
        /*
        create the body first, giving it a position
        */
		b2BodyDef bodyDef;
		bodyDef.position.Set(0.0f, -6.1f);
        /*
         body is created in the world
         */
        b2Body* groundBody = world.CreateBody(&bodyDef);
        
        /*
         create a shape(box for this instance)
         */
		b2PolygonShape groundBox;
		b2PolygonShape wallBox;
		groundBox.SetAsBox(100.0f, 0.5f);
		wallBox.SetAsBox(0.3f, 100.0f);

		
        /*
         assign the box shape and density to the body
         When you attach a shape to a body using a fixture, the shape's coordinates become local to the body. So
         when the body moves, so does the shape.
         */
        groundBody->CreateFixture(&groundBox, 0.0f);
		bodyDef.position.Set(0.0f, 6.1f);
		groundBody = world.CreateBody(&bodyDef);
		groundBody->CreateFixture(&groundBox, 0.0f);

		
		bodyDef.position.Set(-8.7f, 0.0f);
		groundBody = world.CreateBody(&bodyDef);
		groundBody->CreateFixture(&wallBox, 0.0f);
		bodyDef.position.Set(8.7f, 0.0f);
		groundBody = world.CreateBody(&bodyDef);
		groundBody->CreateFixture(&wallBox, 0.0f);
	}

	~WorldPhysics()
	{

	}


	/**************************************************************************
	*
	*	remove all the objects in the world
	*
	**************************************************************************/
	void clearWorld() 
	{
			int pos = 0;
			for (auto bodyObject = world.GetBodyList(); bodyObject
				; )
			{
					b2Body* oldBody = bodyObject;
					bodyObject = bodyObject->GetNext();
					world.DestroyBody(oldBody);
					oldBody = NULL;
				
			}
		
	}

	/**************************************************************************
	*
	*	remove the object at a given index position in the bodyList
	*	remember that the ground is the first object created
	**************************************************************************/
	void removeObj(int index)
	{
		if (index < world.GetBodyCount())
		{
			int pos = 0;
			for (b2Body* bodyObject = world.GetBodyList() ; bodyObject || pos < index;++pos)
			{
				b2Body* oldBody = bodyObject;
				bodyObject = bodyObject->GetNext();
				if (pos == index)
				{
					world.DestroyBody(oldBody);
				}
			}
		}
	}

	/**************************************************************************
	*
	*	Progress through the world using a set amount of time(timestep), and
	*	Iterate through calculations for velocity and position the set 
	*	number of iterations.
	**************************************************************************/
	void Step()
	{
		world.Step(this->timeStep, this->velocityIterations, this->positionIterations);
	}

	/**************************************************************************
	*
	*	Progress through the world using a frameTimeStep, and
	*	Iterate through calculations for velocity and position the set
	*	number of iterations.
	**************************************************************************/
	void Step(float32 timeStep)
	{
		world.Step(timeStep, this->velocityIterations, this->positionIterations);
	}
	/**************************************************************************
	*
	*	return access to the private world
	*
	**************************************************************************/
	b2World* getWorld()
	{
		return &world;
	}

	/**************************************************************************
	*
	*	set the gravity
	*
	**************************************************************************/
	void setGravity(GLfloat gravx,GLfloat gravy)
	{
		world.SetGravity(b2Vec2(gravx,gravy));
	}
	/**************************************************************************
	*
	*	set the gravity
	*
	**************************************************************************/
	float32 getTimeStep()
	{
		return timeStep;
	}
	/**************************************************************************
	*
	*	clear all customized forces in the world
	*
	**************************************************************************/
	void clearForces()
	{
		world.ClearForces();
	}

private:
	b2Vec2 gravity;
	b2World world;
	juce::int32 velocityIterations;
	juce::int32 positionIterations;
	float32 timeStep;
};
