#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "WorldPhysics.h"
#include "Level.h"
#include "RenderableObject.h"

/** A singleton class which represents the data model for a game. This includes
    all Levels, Scenes, GameObjects, etc that describe a game. The GameModel
    data is manipulated by the GameLogic class, and data is copied into
    renderable frames for the GameView to render.
 */
class GameModel{

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

	void addLevel(String levelName) {
		levels.add(new Level(levelName));
	}
    
    void removeLevel(int levelIndex)
    {
        if (levels.size() > 1 && levelIndex < levels.size())
        {
            levels.remove(levelIndex);
            setCurrentLevel(0);
        }
    }

	Level * getCurrentLevel()
	{
		return levels[currentLevel];
	}
    
    void setCurrentLevel (int levelIndex)
    {
        currentLevel = levelIndex;
    }
    
    int getCurrentLevelIndex()
    {
        return currentLevel;
    }
    
    int getNumLevels()
    {
        return levels.size();
    }

private:
	OwnedArray<Level> levels;
	int currentLevel;
	
};
