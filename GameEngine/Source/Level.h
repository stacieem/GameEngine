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
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk01.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk02.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk03.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk04.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk05.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk06.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk07.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk08.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk09.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk10.png"));
		player->addAnimationTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_walk11.png"));

		player->setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));


		player->setCanimate(true);
		//player->setIsAnimating(true);
		//player->setAnimationStartTime(Time::currentTimeMillis());
		player->setAnimationTotalTime(450);

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