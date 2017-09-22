#pragma once


#include "../JuceLibraryCode/JuceHeader.h"

class WorldPhysics {

public:
	b2Vec2 gravity;
	b2World world;
	juce::int32 velocityIterations;
	juce::int32 positionIterations;
	float32 timeStep;

	WorldPhysics() : gravity(0.0f, -10.0f), world(gravity)
	{
		velocityIterations = 2.0f;
		positionIterations = 6.0f;
		timeStep = 1.0f / 60.0f;
        
        /*
        create the body first, giving it a position
        */
        b2BodyDef groundBodyDef;
        groundBodyDef.position.Set(0.0f, -2.0f);
        /*
         body is created in the world
         */
        b2Body* groundBody = world.CreateBody(&groundBodyDef);
        
        /*
         create a shape(box for this instance)
         */
        b2PolygonShape groundBox;
        groundBox.SetAsBox(50.0f, 0.5f);
        
        /*
         assign the box shape and density to the body
         When you attach a shape to a body using a fixture, the shapeís coordinates become local to the body. So
         when the body moves, so does the shape.
         */
        groundBody->CreateFixture(&groundBox, 0.0f);

	}

	~WorldPhysics()
	{

	}

	void Step()
	{
		world.Step(timeStep, velocityIterations, positionIterations);
	}
    
    b2World & getWorld()
    {
        return world;
    }
};
