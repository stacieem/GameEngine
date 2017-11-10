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

		PlayerObject* player = new PlayerObject(worldPhysics, modelsForRendering[0]);
		player->setScore(0);
		player->setLives(1);
		player->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));
		

		player->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));


		player->getRenderableObject().animationProperties.setCanimate(true);


		gameObjects.add(player);
		players.add(player);

		enemyPoints = 15;
		collectablePoints = 5;
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
        gameObj->setModel(modelsForRendering[0]);
		gameObj->setScore(10);
        gameObj->setScale(1.0f, 1.0f);
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
		enm->setModel(modelsForRendering[0]);
		enm->setScale(1.0f, 1.0f);
		gameObjects.add(enm);
	}
    
	void addNewCollectable()
    {
		CollectableObject* collectable = new CollectableObject(worldPhysics, modelsForRendering[0]);

		collectable->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/coin.png"));
		collectable->setScore(5);
		collectable->setModel(modelsForRendering[0]);
		collectable->setScale(1.0f, 1.0f);
		gameObjects.add(collectable);
	}
    
	void addNewCheckpoint()
    {
		checkpoint = new GoalPointObject(worldPhysics, modelsForRendering[0]);

		checkpoint->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/checkpoint.png"));

		checkpoint->setModel(modelsForRendering[0]);
		checkpoint->setScale(1.0f, 1.0f);
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
		for (auto  obj : gameObjects) {
			obj->setPositionWithPhysics(obj->getOrigin().x, obj->getOrigin().y);
			obj->getPhysicsProperties().setLinearVelocity(0, 0);
			obj->setActive(true);
			obj->setRenderable(true);
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
    
    /** Gets the game objects in the specified 2D range in world space.
        The range is 2 diagonal coordinates of a square.
     */
    Array<GameObject *> getObjectsInRange (glm::vec2 coord1, glm::vec2 coord2)
    {
        // Calculate mins and max of range from points
        float xMin;
        float xMax;
        float yMin;
        float yMax;
        
        if (coord1.x <= coord2.x)
        {
            xMin = coord1.x;
            xMax = coord2.x;
        }
        else
        {
            xMin = coord2.x;
            xMax = coord1.x;
        }
        
        if (coord1.y <= coord2.y)
        {
            yMin = coord1.y;
            yMax = coord2.y;
        }
        else
        {
            yMin = coord2.y;
            yMax = coord1.y;
        }
        
        Array<GameObject *> objectsInRange;
        
        // For all game objects, if they are in range, add them
        for (GameObject * gameObject : gameObjects)
        {
            if (gameObject->isInRange (xMin, xMax, yMin, yMax))
                objectsInRange.add (gameObject);
        }
        
        return objectsInRange;
    }
	
	//clean this up
	//Score properties
	void setScoreEnabled() {
		hasScore = !hasScore;
	}
	bool isScoreEnabled() {
		return hasScore;
	}
	int getEnemyPoints() {
		return enemyPoints;
	}
	int getCollectablePoints() {
		return collectablePoints;
	}
	void setEnemyPoints(int points) {
		enemyPoints = points;
	}
	void setCollectablePoints(int points) {
		collectablePoints = points;
	}
	
    
    /** Deletes a single object from the Level.
     */
    void deleteObject (GameObject * gameObjectToDelete)
    {
        // Find the object in the level's object's array
        int indexOfObject = gameObjects.indexOf(gameObjectToDelete);
        
        // If object was found in array (it should be)
        if (indexOfObject != -1)
        {
            // Remove the object from the physics world
            worldPhysics.removeObject (gameObjectToDelete->getPhysicsProperties().getBody());
            
            // Remove the object from the level
            gameObjects.remove(indexOfObject);
        }
    }
    
    /** Deletes a set of objects from the Level.
     */
    void deleteObjects (Array<GameObject *> gameObjectsToDelete)
    {
        // For every object to delete, delete it
        for (GameObject * objectToDelete : gameObjectsToDelete)
        {
            deleteObject(objectToDelete);
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
	int enemyPoints, collectablePoints;
	float Time, timerSpeed;
	bool hasScore, hasTimer, hasCheckpoint;
	int score, level;
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
