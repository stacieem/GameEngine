#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "WorldPhysics.h"
#include "Level.h"
/** Represents the data model for a game. This includes all Levels, Scenes, 
    GameObjects, etc that describe a game. The data is manipulated by the
    GameLogic class and is rendered by the GameView class.
 */
class GameModel {

public:

	GameModel()
	{
		// Initialize Current Level
		levels.add(new Level("Tha Swag"));
		currentLevel = 0;
	}

	~GameModel(){}


	/*
		int levelNumber passed in is the index in the array
	*/
	Level & getLevel(int levelNumber) {
		return *levels[levelNumber];
	}

	void addNewLevel(String levelName) {
		levels.add(new Level(levelName));
	}

	Level & getCurrentLevel()
	{
		return *levels[currentLevel];
	}

private:
	OwnedArray<Level> levels;
	int currentLevel;

};
