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

/*
	Abstract button assignments to keep magical indeces out of this
*/
class InputManager : public KeyListener, public MouseListener {
public:

	InputManager(){	}

	bool keyPressed(const KeyPress& key, Component* originatingComponent) {
		if ((keyboardBinding.find(key.getTextCharacter()) != keyboardBinding.end())) {
			if (commands.indexOf(keyboardBinding[key.getTextCharacter()]) == -1) {
				commands.add(keyboardBinding[key.getTextCharacter()]);
			}
		}

		if ((keyboardBinding2.find(key.getTextCharacter()) != keyboardBinding2.end())) {
			if (commands2.indexOf(keyboardBinding2[key.getTextCharacter()]) == -1) {
				commands2.add(keyboardBinding2[key.getTextCharacter()]);
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
	// addCommands for a keyboard, might want to pass diff param for other inputs
	void addCommand2(KeyPress key, GameCommand command) {
		if (keyboardBinding2.find(key.getKeyCode()) == keyboardBinding2.end()) {
			keyboardBinding2[key.getKeyCode()] = command;
		}
	}
	// give access to commands bool vector
	void getCommands1(Array<GameCommand>& newCommands) {
		newCommands = commands;
		commands.clear();
	}
	// give access to commands bool vector
	void getCommands2(Array<GameCommand>& newCommands) {
		newCommands = commands2;
		commands2.clear();
	}
	// mouse has limited controls, looking at possible hard code.
	void mouseDown(const MouseEvent& event){
		//forcibly reset bit position 5 in commands
		if (commands.indexOf(GameCommand::reset) == -1) {
			commands.add(GameCommand::reset);
		}
		if (commands2.indexOf(GameCommand::reset) == -1) {
			commands2.add(GameCommand::reset);
		}
	}

private:
	std::map<juce_wchar, GameCommand> keyboardBinding;
	std::map<juce_wchar, GameCommand> keyboardBinding2;
	Array<GameCommand> commands;
	Array<GameCommand> commands2;
};
