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
		PlayerObject* player = new PlayerObject(worldPhysics, modelsForRendering[0]);

		player->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));
	
		player->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));
		player->getRenderableObject().animationProperties.setCanimate(true);


		gameObjects.add(player);
		players.add(player);


        player->setModel(modelsForRendering[0]);

		score = 0;
		enemyPoints = 0;
		collectablePoints = 0;
		timerSpeed = 0;
		hasTimer = false;
		hasScore = false;
		hasCheckpoint = false;
	}

	Level(ValueTree levelValueTree) {

		modelsForRendering.add(new Model());

		parseFrom(levelValueTree);
	}

    
	~Level()
    {
        
    }

	int getNumGameObjects() {
		return gameObjects.size();
	}

	void addNewPlayer() {
		players.add(new PlayerObject(worldPhysics, modelsForRendering[0]));
	}

	void addNewObject() {
		gameObjects.add(new GameObject(worldPhysics, modelsForRendering[0]));
	}

	void addNewBlock() {
        GameObject * gameObj = new GameObject(worldPhysics, modelsForRendering[0]);

		gameObj->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/brick.png"));
		gameObjects.add(gameObj);
	}
    
    GameObject * copyObject(GameObject * objectToCopy) {
        GameObject * newObject = new GameObject(*objectToCopy, worldPhysics);
        gameObjects.add(newObject);
        return newObject;
    }
    
	void addNewEnemy() {
		EnemyObject* enm = new EnemyObject(worldPhysics, modelsForRendering[0]);
		enm->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/blue_alien/walk/"));

		enm->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/blue_alien/p2_stand.png"));


		enm->getRenderableObject().animationProperties.setCanimate(true);
		gameObjects.add(enm);
	}
    
	void addNewCollectable()
    {
		CollectableObject* collectable = new CollectableObject(worldPhysics, modelsForRendering[0]);

		collectable->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/coin.png"));

		gameObjects.add(collectable);
	}
    
	void addCheckpoint()
    {
		checkpoint = new GoalPointObject(worldPhysics, modelsForRendering[0]);

		checkpoint->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/checkpoint.png"));

		gameObjects.add(checkpoint);
	}

	void removeCheckpoint()
    {
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
    
	//reset the current level to an original state
	void resetLevel() {
		DBG("resetLevel");
		for (auto  obj : gameObjects) {
			DBG("obj origin: " << obj->getOrigin().x << " " << obj->getOrigin().y);
			DBG("obj position: " << obj->getPhysicsProperties().GetPosition().x << " " << obj->getPhysicsProperties().GetPosition().y);
			
			obj->setPositionWithPhysics(obj->getOrigin().x, obj->getOrigin().y);
			obj->getPhysicsProperties().setLinearVelocity(0, 0);
		}
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
    
    /** Gets the game object at the position in world space.
        (This essentially casts a ray into the scene and determines what object
        it collides with, but since this is just 2D it is a bit simpler than that,
        just the 2D position is evaluated)
     
        @return GameObject at the given position or nullptr if no object found
     */
    GameObject * getObjectAtPosition(glm::vec2 position)
    {
        // Iterate through GameObjects and find one that fits posiion
        for (GameObject * gameObject : gameObjects)
        {
            if (gameObject->isAtPosition(position))
                return gameObject;
        }
        
        return nullptr;
    }
	
	//clean this up
	//Score properties
	void setScoreEnabled() {
		hasScore = !hasScore;
	}
	bool isScoreEnabled() {
		return hasScore;
	}
	int getScore() {
		return score;
	}
	int getEnemyPoints() {
		return enemyPoints;
	}
	int getCollectablePoints() {
		return collectablePoints;
	}
	void setScore(int baseScore) {
		score = baseScore;
	}
	void setEnemyPoints(int points) {
		enemyPoints = points;
	}
	void setCollectablePoints(int points) {
		collectablePoints = points;
	}
	void addToScore(int points) {
		score += points;
	}
	
	//timer properties
	bool isTimerEnabled() {
		return hasTimer;
	}
	void setTimerEnabled() {
		hasTimer = !hasTimer;
	}
	int getTimer() {
		return time;
	}
	void setTimer(float baseTimer) {
		score = baseTimer;
	}
	void setTimerSpeed(float timeSpeed) {
		timerSpeed = timeSpeed;
	}
	void decrementTimer(float decrement) {
		time -= decrement;
	}
	
	//checkpoint properties
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

	int getGravityState() {
		return worldPhysics.getGravityLevel();
	}

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the game
		ValueTree levelSerialization = ValueTree("Level");

		levelSerialization.setProperty(Identifier("name"), var(levelName), nullptr);

		//Serialize camera position
		levelSerialization.addChild(camera.serializeToValueTree(), -1, nullptr);

		//Serialize world physics
		levelSerialization.addChild(worldPhysics.serializeToValueTree(), -1, nullptr);

		//Serialize models
		//Just kidding. we only have one default model, no need to serialize

		//Get the player index in the game model while we are serializing game objects
		ValueTree playerValueTree = ValueTree("Player");

		ValueTree gameObjectsValueTree = ValueTree("GameObjects");

		//For each level, serialize and add to Levels child element
		for (GameObject* gameObject : gameObjects) {

			//If this is the player, set the player index to be this index
			if (gameObject == players[0]) {
				playerValueTree.setProperty(Identifier("index"), var(gameObjects.indexOf(gameObject)), nullptr);
				gameObjectsValueTree.addChild(((PlayerObject*)gameObject)->serializeToValueTree(), -1, nullptr);
			}
			else if (gameObject->getObjType() == Enemy) {
				gameObjectsValueTree.addChild(((EnemyObject*)gameObject)->serializeToValueTree(), -1, nullptr);
			}
			else {
				gameObjectsValueTree.addChild(gameObject->serializeToValueTree(), -1, nullptr);
			}
		}

		levelSerialization.addChild(gameObjectsValueTree, -1, nullptr);

		levelSerialization.addChild(playerValueTree, -1, nullptr);

		return levelSerialization;
	}

	void parseFrom(ValueTree levelTree) {

		camera.parseCameraFrom(levelTree.getChildWithName(Identifier("Camera")));

		ValueTree gameObjectsValueTree = levelTree.getChildWithName(Identifier("GameObjects"));

		ValueTree worldPhysicsValueTree = levelTree.getChildWithName(Identifier("WorldPhysics"));

		worldPhysics.parseWorldPhysics(worldPhysicsValueTree);

		for (ValueTree gameObjectValueTree : gameObjectsValueTree) {

			int objTypeInt = gameObjectValueTree.getProperty(Identifier("type"));

			switch (objTypeInt) {

			case 0: {
				GameObject* genericObj = new GameObject(worldPhysics, modelsForRendering[0], gameObjectValueTree);
				gameObjects.add(genericObj);
			}
					break;
			case 1: {
				PlayerObject* player = new PlayerObject(worldPhysics, modelsForRendering[0], gameObjectValueTree);
				gameObjects.add(player);
				players.add(player);

			}
					break;
			case 2: {
				EnemyObject* enm = new EnemyObject(worldPhysics, modelsForRendering[0], gameObjectValueTree);
				gameObjects.add(enm);
			}
					break;
			case 3: {
				CollectableObject* collectable = new CollectableObject(worldPhysics, modelsForRendering[0], gameObjectValueTree);
				gameObjects.add(collectable);
			}
				break;
			case 4: {
				GoalPointObject* goalPoint = new GoalPointObject(worldPhysics, modelsForRendering[0], gameObjectValueTree);
				gameObjects.add(goalPoint);
			}
				break;
			}


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
	int score, enemyPoints, collectablePoints;
	float time, timerSpeed;
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

