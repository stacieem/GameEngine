#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "WorldPhysics.h"

/** Represents the data model for a game. This includes all Levels, Scenes, 
    GameObjects, etc that describe a game. The data is manipulated by the
    GameLogic class and is rendered by the GameView class.
 */
class GameModel {

public:

	GameModel()
    {
		gameObjects.add (new GameObject(worldPhysics));
        
//        gameObjects.add (new GameObject(worldPhysics));
//        gameObjects.getLast()->translate(0.0, 3.0);
//        
//        gameObjects.add (new GameObject(worldPhysics));
//        gameObjects.getLast()->translate(2.0, 0.0);
	}
    
	~GameModel()
    {
        
	}
    
    const OwnedArray<GameObject> & getGameObjects()
    {
        return gameObjects;
    }
    
    /** Processes an physics in the world for a given frame in the physics
        timeline
     */
    void processWorldPhysics()
    {
        worldPhysics.Step();
    }
    
    WorldPhysics & getWorldPhyscis()
    {
        return worldPhysics;
    }


private:
    
    // The commands that a user is allowed to use in the game
    //GameCommands
    
    // Eventually this will be encapsulated in Scenes and further those scenes
    // will be encapsulated in Levels
    OwnedArray<GameObject> gameObjects;
    
    //OwnedArray<GameObject> gameObjects;
    
    WorldPhysics worldPhysics;

};
