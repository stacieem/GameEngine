#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "WorldPhysics.h"

/** Represents the data model for a game. This includes all Levels, Scenes, 
    GameObjects, etc that describe a game. The data is manipulated by the
    GameLogic class and is rendered by the GameView class.
 */
class GameModel {

public:

	GameModel()
    {
		player1 = new PlayerObject(worldPhysics);
		gameObjects.add(player1);
		player2 = new PlayerObject(worldPhysics);
		gameObjects.add(player2);
	}
    
	~GameModel()
    {
        
	}
    OwnedArray<GameObject> & getGameObjects()
    {
        return gameObjects;
    }
    
    /** Processes an physics in the world for a given frame in the physics
        timeline
     */
    void processWorldPhysics(int64 timeStep)
    {

		if ((float32)timeStep < worldPhysics.getTimeStep()) {
			worldPhysics.Step((float32)timeStep);
		}
		else
		{
			worldPhysics.Step();
		}
    }
	PlayerObject* getPlayer1() {
		return player1;
	}


	PlayerObject* getPlayer2() {
		return player2;
	}
private:
    
    // The commands that a user is allowed to use in the game
    //GameCommands
    
    // Eventually this will be encapsulated in Scenes and further those scenes
    // will be encapsulated in Levels
    OwnedArray<GameObject> gameObjects;
	PlayerObject* player1;
	PlayerObject* player2;

    //OwnedArray<GameObject> gameObjects;
    
    WorldPhysics worldPhysics;

};
