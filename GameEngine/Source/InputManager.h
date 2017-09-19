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

/*
	Abstract button assignments to keep magical indeces out of this
*/
class InputManager : public Component, public KeyListener, public MouseListener {
public:

	InputManager(){}

	bool keyPressed(const KeyPress& key, Component* originatingComponent) {
		if (keyboardBinding.find(key.getTextCharacter()) != keyboardBinding.end()) {
			commands[keyboardBinding[key.getTextCharacter()]] = true;
		}
		
		return 0;
	}

	// addCommands for a keyboard, might want to pass diff param for other inputs
	void addCommand(juce_wchar key) {
		commands.push_back(false);
		keyboardBinding[key] = commands.size() - 1;
	}
	
	//reset player inputs for next frame
	void reset() {
		for (auto cmd : commands){
			cmd = false;
		}
	}
	// give access to commands bool vector
	std::vector<bool> getCommands() {
		return commands;
	}

	// mouse has limited controls, looking at possible hard code.
	void mouseDown(const MouseEvent& event){
		//forcibly reset bit position 5 in commands
		commands[4] = true;
	}

private:
	std::map<juce_wchar, int> keyboardBinding;
	std::vector<bool> commands;
};