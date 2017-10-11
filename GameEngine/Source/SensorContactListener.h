#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObjectType.h"
#include "GameObject.h"
class SensorContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) override 
	{
		// Get both fixtures
		b2Fixture * fixtureA = contact->GetFixtureA();
		b2Fixture * fixtureB = contact->GetFixtureB();

		// If one of the fixtures is a sensor but not both, then a physical object
		// has entered a sensor range
		if (fixtureA->IsSensor() != fixtureB->IsSensor()) {

			// Get user data for both objects
			void* bodyUserDataA = contact->GetFixtureA()->GetBody()->GetUserData();
			void* bodyUserDataB = contact->GetFixtureB()->GetBody()->GetUserData();

			// If User data could be grabbed for both objects
			// (THIS MUST ALWAYS BE TRUE because the user data is set in the GameObject
			// constructor)
			if (bodyUserDataA && bodyUserDataB)
			{
				// Cast the user data to GameObjects
				//GameObject * objectA = static_cast<GameObject*>(bodyUserDataA);
				//GameObject * objectB = static_cast<GameObject*>(bodyUserDataB);

				// If either object is a Player, then trigger the sensor action
				// for the particular object that had its sensor triggered
				/*if (objectA->getObjType() == GameObjectType::Player
					|| objectB->getObjType() == GameObjectType::Player)
				{
					DBG("Player entered a sensor zone.");
				}
				*/
				//DBG("Player entered a sensor zone.");
			}

		}
	}

	void EndContact(b2Contact* contact) override 
	{
		//DBG("ContactEnd");
	}
};