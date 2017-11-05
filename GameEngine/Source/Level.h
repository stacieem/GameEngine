#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "GoalPointObject.h"
#include "GameObject.h"
#include "CollectableObject.h"
#include "Camera.h"

class Level {
public:
	Level(String levelName) {
        
        // Add a default model
        modelsForRendering.add(new Model());
        
		this->levelName = levelName;
		// Trystan's Multiplayer Test
		PlayerObject* player = new PlayerObject(worldPhysics);

		//player->addTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/alienBlue_stand.png"));
		player->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));
		

		player->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));


		player->getRenderableObject().animationProperties.setCanimate(true);
		//player->setIsAnimating(true);
		//player->setAnimationStartTime(Time::currentTimeMillis());

		gameObjects.add(player);
		players.add(player);


        player->setModel(modelsForRendering[0]);

		Score = 0;
		enemyPoints = 0;
		collectablePoints = 0;
		timerSpeed = 0;
		hasTimer = false;
		hasScore = false;
		hasCheckpoint = false;


	}
    
	~Level()
    {
        
    }

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
        GameObject * gameObj = new GameObject(worldPhysics);
        gameObj->setModel(modelsForRendering[0]);
        gameObj->setScale(1.0f, 1.0f);
		gameObjects.add(gameObj);
	}
	void addNewEnemy() {
		EnemyObject* enm = new EnemyObject(worldPhysics);
		enm->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));

		enm->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));


		enm->getRenderableObject().animationProperties.setCanimate(true);
		enm->setModel(modelsForRendering[0]);
		enm->setScale(1.0f, 1.0f);
		gameObjects.add(enm);
	}
	void addNewCollectable() {
		CollectableObject* collectable = new CollectableObject(worldPhysics);

		collectable->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/coin.png"));

		collectable->setModel(modelsForRendering[0]);
		collectable->setScale(1.0f, 1.0f);
		gameObjects.add(collectable);
	}
	void addCheckpoint() {
		checkpoint = new GoalPointObject(worldPhysics);

		checkpoint->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/checkpoint.png"));

		checkpoint->setModel(modelsForRendering[0]);
		checkpoint->setScale(1.0f, 1.0f);
		gameObjects.add(checkpoint);
	}

	void removeCheckpoint() {
		int i = 0;
		for (auto obj : gameObjects) {
			
			if (obj->getObjType() == Checkpoint) {
				gameObjects.remove(i, true);
			}
			i++;
		}
	}
	const OwnedArray<GameObject> & getGameObjects()
	{
		return gameObjects;
	}

	//Return a player Object
	PlayerObject * getPlayer(int playerNumber)
    {
		return players[playerNumber];
	}

	//Return an object in this World
	GameObject & getObject(int ObjectID)
    {
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
        
        updateObjectsPositionsFromPhysics();
	}
    
    
	//Return the WorldPhysics for this level
	WorldPhysics & getWorldPhysics()
    {
		return worldPhysics;
	}
    
    Camera & getCamera()
    {
        return camera;
    }
	
	//clean this up
	void setScoreEnabled() {
		hasScore = !hasScore;
	}
	bool isScoreEnabled() {
		return hasScore;
	}
	int getScore() {
		return Score;
	}
	void setScore(int baseScore) {
		Score = baseScore;
	}
	int getEnemyPoints() {
		return enemyPoints;
	}
	void setEnemyPoints(int points) {
		enemyPoints = points;
	}
	int getCollectablePoints() {
		return collectablePoints;
	}
	void setCollectablePoints(int points) {
		collectablePoints = points;
	}
	void addToScore(int points) {
		Score += points;
	}
	bool isTimerEnabled() {
		return hasTimer;
	}
	void setTimerEnabled() {
		hasTimer = !hasTimer;
	}
	int getTimer() {
		return Time;
	}
	void setTimer(float baseTimer) {
		Score = baseTimer;
	}
	void setTimerSpeed(float timeSpeed) {
		timerSpeed = timeSpeed;
	}
	void decrementTimer(float decrement) {
		Time -= decrement;
	}
	bool isCheckpointEnabled() {
		return hasCheckpoint;
	}
	void setCheckpointEnabled() {
		hasCheckpoint = !hasCheckpoint;
		if (hasCheckpoint) {
			addCheckpoint();
		}
		else
		{
			removeCheckpoint();
		}
	}

private:
    
    /** Updates positions from all objects from the Physics updates
     */
    void updateObjectsPositionsFromPhysics()
    {
        for (auto object : gameObjects)
        {
            b2Vec2 box2DPos = object->getPhysicsProperties().GetPosition();
            object->setPosition(box2DPos.x, box2DPos.y);
        }
    }
    
    /** Name of level */
    String levelName;
	int Score, enemyPoints, collectablePoints;
	float Time, timerSpeed;
	bool hasScore, hasTimer, hasCheckpoint;
   
	/** Camera view of the current level */
    Camera camera;
	GoalPointObject* checkpoint;
    /** Physics for the level */
    WorldPhysics worldPhysics;
    
    /** GameObjects in the level */
	OwnedArray<GameObject> gameObjects;
    
    /** Set of Models to be used by RenderableObjects inside of GameObjects.
     
    !!! This might be the wrong location for these, but we need a single place
        to keep all the Model's that are currently being used in the level.
        (consider later including this in ResourceManager)

        Why do this?
        Consider multiple objects all using the same Model. For example, pretend
        you have a 3D model of an enemy. Pretend you want to spawn 30 enemies
        that all look alike. All 30 GameObjects that are the enimies will all
        point to the same Model. This model does not need to be stored multiple
        times since each enemy is using the same Model data. The only thing that
        must differ between enemies is the position, rotation, etc. which is why
        each Renderable Object has its own model-matrix to position it.
     */
    OwnedArray<Model> modelsForRendering;
    
    /** Game Players in the level */
	Array<PlayerObject *> players;
};
