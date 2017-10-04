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
        // Demo 1 Basic Game Objects (breaks cuz of hard coded texture map)
        //gameObjects.add(new GameObject(worldPhysics));
        
        
        /** Demo 1 (Player Object with one Texutre and audio on collisions) */
//        player1 = new PlayerObject(worldPhysics);
//        player1->setTexture("Kenny");
//        gameObjects.add(player1);
//        
//        player2 = new PlayerObject(worldPhysics);
//        player2->setTexture("Kenny");
//        gameObjects.add(player2);
        
        
        /** Demo 2 (2 Players with Texture and different textures) */
//        player1 = new PlayerObject(worldPhysics);
//        player1->setTexture("Kenny");
//        gameObjects.add(player1);
//        
//        player2 = new PlayerObject(worldPhysics);
//        player2->setTexture("Flower");
//        gameObjects.add(player2);
//
        
        /** Demo 3 (2 Players with Texture and different audio) */
        player1 = new PlayerObject(worldPhysics);
        player1->setTexture("Kenny");
        player1->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/Hey.wav"), PhysicalAction::collsion);
        gameObjects.add(player1);
        
        player2 = new PlayerObject(worldPhysics);
        player2->setTexture("Flower");
        player2->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/Air Horn.wav"), PhysicalAction::collsion);
        gameObjects.add(player2);
        
        
        /** Demo 4 (lots and lots of objects) */
        
//        player1 = new PlayerObject(worldPhysics);
//        player1->setTexture("Kenny");
//        player1->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/Hey.wav"), PhysicalAction::collsion);
//        gameObjects.add(player1);
//        
//        player2 = new PlayerObject(worldPhysics);
//        player2->setTexture("Flower");
//        player2->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/YooHoo.wav"), PhysicalAction::collsion);
//        gameObjects.add(player2);
//        
//        for (int i = 0; i < 200; i++) {
//            GameObject* obj = new GameObject(worldPhysics);
//            obj->setTexture("Flower");
//            obj->getPhysicsProperties().setRestitution(1.5);
//            gameObjects.add(obj);
//        }
        
        /** Demo 5 (lots and lots of objects & physics changes) */
//        player1 = new PlayerObject(worldPhysics);
//        player1->setTexture("Kenny");
//        player1->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/Hey.wav"), PhysicalAction::collsion);
//        gameObjects.add(player1);
//
//        player2 = new PlayerObject(worldPhysics);
//        player2->setTexture("Flower");
//        player2->mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/YooHoo.wav"), PhysicalAction::collsion);
//        gameObjects.add(player2);
//
//        for (int i = 0; i < 2; i++) {
//            GameObject* obj = new GameObject(worldPhysics);
//            obj->setTexture("Flower");
////            obj->getPhysicsProperties().setRestitution(1.5);
////            obj->getPhysicsProperties().setDensity(.5);
////            obj->getPhysicsProperties().setFriction(.3);
//            gameObjects.add(obj);
//        }
        
        
        
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
