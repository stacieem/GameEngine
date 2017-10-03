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

        // Morgan's Texure Test
//		player = new PlayerObject(worldPhysics);
//		gameObjects.add(player);
//		gameObjects[0]->setTexture("Kenny");
//        
//        gameObjects.add (new GameObject(worldPhysics));
//        gameObjects[1]->setTexture("Flower");
//
//		gameObjects.add(new GameObject(worldPhysics));
//		gameObjects[2]->setTexture("Kenny");
//		gameObjects[2]->translateBy(2, 2);
//
        
        // Tim's Audio Test
        
        //        gameObjects.add (new GameObject(worldPhysics));
        //        gameObjects.getLast()->translate(0.0, 3.0);
        //
        //        gameObjects.add (new GameObject(worldPhysics));
        //        gameObjects.getLast()->translate(2.0, 0.0);
        
        // Trystan's Multiplayer Test
		player1 = new PlayerObject(worldPhysics);
        player1->setTexture("Flower");
		gameObjects.add(player1);
        
		player2 = new PlayerObject(worldPhysics);
        player2->setTexture("Kenny");
		gameObjects.add(player2);
		/*
		for (int i = 0; i < 200; i++) {
			GameObject* obj = new GameObject(worldPhysics);
			obj->setTexture("Flower");
			obj->getPhysicsProperties().setRestitution(1.5f);
			obj->getPhysicsProperties().setFriction(.3f);
			obj->getPhysicsProperties().setDensity(.5f);
			gameObjects.add(obj);
		}
		*/
	}
    
	~GameModel()
    {
        
	}
    
    const OwnedArray<GameObject> & getGameObjects()
    {
        return gameObjects;
    }

	int getNumGameObjects() {
		return gameObjects.size();
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
    
    WorldPhysics & getWorldPhyscis()
    {
        return worldPhysics;
    }

	PlayerObject* getPlayer1() {
		return player1;
	}


	PlayerObject* getPlayer2() {
		return player2;
	}

	// add new game Object
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
