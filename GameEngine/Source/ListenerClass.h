#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
class ListenerClass : public b2ContactListener{
public:
		void endContact(b2Contact* contact) {

		}

		void beginContact(b2Contact* contact) {
			void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
			if (bodyUserData) {
				DBG("Contacted2");
			}
			DBG("Contacted");
			if (((GameObject*)bodyUserData)->getObjType() == Player) {
				bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
				switch (((GameObject*)bodyUserData)->getObjType()) {
					case Enemy:
							//lose lives
						DBG("Enemy");
						break;
					case Collectable:
							//addToScore
						DBG("Collectable");
						break;
					case Checkpoint:
							//go to nextLevel
						DBG("CheckPoint");
						break;
				}
			}

	}
private:

};