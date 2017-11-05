#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "GameObject.h"
#include "Camera.h"

class Level {
public:
	Level(String levelName) {
        
        // Add a default model
        modelsForRendering.add(new Model());
        
		this->levelName = levelName;
		// Trystan's Multiplayer Test
		PlayerObject* player = new PlayerObject(worldPhysics);

		player->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/walk/"));
		
		player->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/alien/p1_stand.png"));

		player->getRenderableObject().animationProperties.setCanimate(true);

		gameObjects.add(player);
		players.add(player);

        player->setModel(modelsForRendering[0]);

	}

	Level(ValueTree levelValueTree) {

		modelsForRendering.add(new Model());

		parseLevel(levelValueTree);
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
		enm->getRenderableObject().animationProperties.setAnimationTextures(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/blue_alien/walk_p2/"));

		enm->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/blue_alien/p2_stand.png"));


		enm->getRenderableObject().animationProperties.setCanimate(true);
		enm->setModel(modelsForRendering[0]);
		enm->setScale(1.0f, 1.0f);
		gameObjects.add(enm);
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

	String getName() {
		return levelName;
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
			} 			else if (gameObject->getObjType() == Enemy)	{
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

	void parseLevel(ValueTree levelTree) {

		camera.parseCameraFrom(levelTree.getChildWithName(Identifier("Camera")));

		ValueTree gameObjectsValueTree = levelTree.getChildWithName(Identifier("GameObjects"));

		ValueTree worldPhysicsValueTree = levelTree.getChildWithName(Identifier("WorldPhysics"));

		worldPhysics.parseWorldPhysics(worldPhysicsValueTree);

		for (ValueTree gameObjectValueTree : gameObjectsValueTree) {

			int objTypeInt = gameObjectValueTree.getProperty(Identifier("type"));

			switch (objTypeInt) {

			case 0: {
				GameObject* genericObj = new GameObject(worldPhysics, gameObjectValueTree);
				genericObj->setModel(modelsForRendering[0]);
				genericObj->setScale(1.0f, 1.0f);
				gameObjects.add(genericObj);
			}
				break;
			case 1: {
				PlayerObject* player = new PlayerObject(worldPhysics, gameObjectValueTree);
				player->setModel(modelsForRendering[0]);
				gameObjects.add(player);
				players.add(player);
				
			}
					break;
			case 2: {
				EnemyObject* enm = new EnemyObject(worldPhysics, gameObjectValueTree);
				enm->setModel(modelsForRendering[0]);
				enm->setScale(1.0f, 1.0f);
				gameObjects.add(enm);
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
    
    /** Camera view of the current level */
    Camera camera;
    
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
