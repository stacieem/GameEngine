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
		name = "Game of JUCE";

		// Initialize Current Level
		levels.add(new Level("Tha Swag"));
		currentLevel = 0;
	}


	GameModel(File file) {
	
		XmlDocument doc(file);
	
		ScopedPointer<XmlElement> elem = doc.getDocumentElement();

		parseGameModelXml(elem);

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


	void parseGameModelXml(XmlElement* rootElement) {

		ValueTree gameModelValueTree = ValueTree::fromXml(*rootElement);

		name = gameModelValueTree.getProperty(Identifier("name"));

		ValueTree levelsValueTree = gameModelValueTree.getChildWithName(Identifier("Levels"));

		currentLevel = levelsValueTree.getProperty(Identifier("currentlevel"));

		for (ValueTree levelTree : levelsValueTree) {
			levels.add(new Level(levelTree));
		}

	}

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the game
		ValueTree gameSerialization("Game");

		gameSerialization.setProperty(Identifier("name"),var(name),nullptr);

		ValueTree levelsValueTree("Levels");

		levelsValueTree.setProperty(Identifier("currentlevel"), var(currentLevel), nullptr);

		//For each level, serialize and add to Levels child element
		for (Level* level : levels) {

			levelsValueTree.addChild(level->serializeToValueTree(), -1, nullptr);
		}

		gameSerialization.addChild(levelsValueTree, -1, nullptr);

		return gameSerialization;
	}


	OwnedArray<Level>& getLevels() {
		return levels;
	}

private:
	OwnedArray<Level> levels;
	int currentLevel;

	String name;

};
