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

	bool keyPressed(const KeyPress& key, Component* originatingComponent) {
		if ((keyboardBinding.find(key.getTextCharacter()) != keyboardBinding.end())) {
			if (commands.indexOf(keyboardBinding[key.getTextCharacter()]) == -1) {
				commands.add(keyboardBinding[key.getTextCharacter()]);
			}
		}

		return 0;
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
		commands.clear();
	}

	// mouse has limited controls, looking at possible hard code.
	void mouseDown(const MouseEvent& event){
		//forcibly reset bit position 5 in commands
		if (commands.indexOf(GameCommand::reset) == -1) {
			commands.add(GameCommand::reset);
		}
	}

private:
	std::map<juce_wchar, GameCommand> keyboardBinding;
	Array<GameCommand> commands;

	JUCE_LEAK_DETECTOR(InputManager)
};
