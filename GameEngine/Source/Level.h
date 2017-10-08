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
		player->setTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/flower.jpg"));
		gameObjects.add(player);
		players.add(player);

		player = new PlayerObject(worldPhysics);
		player->setTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/p2_stand.png"));
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