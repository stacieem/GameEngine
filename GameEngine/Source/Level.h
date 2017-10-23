#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayerObject.h"
#include "GameObject.h"
class Level {
public:
	Level(String levelName) {
		this->levelName = levelName;
		// Trystan's Multiplayer Test
		PlayerObject* player = new PlayerObject(worldPhysics);

		//player->addTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/alienBlue_stand.png"));
		player->setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));
		

		player->setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));


		player->setCanimate(true);
		//player->setIsAnimating(true);
		//player->setAnimationStartTime(Time::currentTimeMillis());

		gameObjects.add(player);
		players.add(player);

		player = new PlayerObject(worldPhysics);

		player->setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/flower.jpg")); 
		player->setCanimate(false);
		gameObjects.add(player);
		players.add(player);
	}

	~Level(){}

	int getNumGameObjects() {
		return gameObjects.size();
	}

	void addNewPlayer() {
		players.add(new PlayerObject(worldPhysics));
	}

	void addNewObject() {
		gameObjects.add(new GameObject(worldPhysics));
	}

	void addNewBlock() {
		gameObjects.add(new GameObject(worldPhysics));
	}

	const OwnedArray<GameObject> & getGameObjects()
	{
		return gameObjects;
	}

	const OwnedArray<PlayerObject> & getPlayers()
	{
		return players;
	}

	//Return a player Object
	PlayerObject * getPlayer(int playerNumber) {
		return players[playerNumber];
	}

	//Return an object in this World
	GameObject & getObject(int ObjectID) {
		return *gameObjects[ObjectID];
	}

	/** Processes an physics in the world for a given frame in the physics
	timeline
	*/
	void processWorldPhysics(int64 timeStep)
	{

		if ((float32)timeStep < getWorldPhysics().getTimeStep()) {
			getWorldPhysics().Step((float32)timeStep);
		}
		else
		{
			getWorldPhysics().Step();
		}
	}
	//Return the WorldPhysics for this level
	WorldPhysics & getWorldPhysics() {
		return worldPhysics;
	}
private:
	OwnedArray<GameObject> gameObjects;
	OwnedArray<PlayerObject> players;
	String levelName;
	WorldPhysics worldPhysics;
};