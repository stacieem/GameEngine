//
//  InputBindings
//
//  Created by Trystan Jasperson.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class InputBindings : public Component, public KeyListener, public MouseListener {
public:
	InputBindings(){
		setWantsKeyboardFocus(true);
	}
	void setInfluence(GameObject* gameObject) {
		if (gameObject != NULL) {
			this->gameObject = gameObject;
		}
	}
	bool keyPressed(const KeyPress& key, Component* originatingComponent) {
		int objIndex = 0;
		DBG(key.getTextCharacter());
		juce_wchar c = key.getTextCharacter();
		if (gameObject != NULL) {

			switch (c) {
			case 'w': gameObject->getPhysicsProperties().setImpulse(0.0f, 8.0f);
				break;
			case 's':gameObject->getPhysicsProperties().setImpulse(0.0f, -8.0f);
				break;
			case 'a':gameObject->getPhysicsProperties().setImpulse(-2.0f, 0.0f);
				break;
			case 'd':gameObject->getPhysicsProperties().setImpulse(2.0f, 0.0f);
				break;
			default:
				break;
			}
		}
		return 0;
	}
	void mouseDown(const MouseEvent& event){
		gameObject->translateTo(0.0f, 1.0f);
		
	}
private:
	GameObject* gameObject;
};