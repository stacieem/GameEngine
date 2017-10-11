//
//  InputBindings
//
//  Created by Trystan Jasperson.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include <map>
#include "GameCommand.h"
#include <Windows.h>
#include <Xinput.h>

/*
	Abstract button assignments to keep magical indeces out of this
*/
class InputManager : public KeyListener, public MouseListener {
public:

	InputManager(){	

	}

	~InputManager() {

	}

	//Must exist!
	bool keyPressed(const KeyPress& key, Component* originatingComponent) {

		return 0;
	}

	bool keyStateChanged(bool isKeyDown, Component *originatingComponent) {

		//Iterate through every key binding we support
		//I don't like this but it's the only way to detect which key is up or down
		for (auto const &keyBind : keyboardBinding) {

			//Create the key out of the keyCode from the map
			KeyPress key = KeyPress(keyBind.first);
			
			if (KeyPress::isKeyCurrentlyDown(keyBind.first)) {

				//If the key is down, we want to add it to the commands array
				commands.addIfNotAlreadyThere(keyboardBinding[key.getKeyCode()]);
			}
			else {

				//If the key is up, we want to remove it
				commands.removeAllInstancesOf(keyboardBinding[key.getKeyCode()]);
			} 

		}
		return true;
	}

	// addCommands for a keyboard, might want to pass diff param for other inputs
	void addCommand(KeyPress key, GameCommand command) {
		if (keyboardBinding.find(key.getKeyCode()) == keyboardBinding.end()) {
			keyboardBinding[key.getKeyCode()] = command;
		}
	}

	// give access to commands 
	void getCommands(Array<GameCommand>& newCommands) {
		newCommands = commands;
		//commands.clear();
	}

	// mouse has limited controls, looking at possible hard code.
	void mouseDown(const MouseEvent& event){
		
	}

private:
	std::map<int, GameCommand> keyboardBinding;
	Array<GameCommand> commands;

	JUCE_LEAK_DETECTOR(InputManager)
};
