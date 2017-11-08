#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayerObject.h"
class ListenerClass : public b2ContactListener{
public:
		void endContact(b2Contact* contact) {

	}

		void beginContact(b2Contact* contact) {
			void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
			if (bodyUserData)
				static_cast<Player*>(bodyUserData)->startContact();

			//check if fixture B was a ball
			bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
			if (bodyUserData)
				static_cast<Ball*>(bodyUserData)->startContact();

	}
private:

};