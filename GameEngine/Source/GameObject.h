//
//  GameObject.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/7/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PhysicsProperties.h"
#include "WorldPhysics.h"
#include "PhysicalAction.h"
#include <map>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "Model.h"
#include "RenderableObject.h"
#include <algorithm>
#include "GameObjectType.h"
#include "GameAudio.h"
/** Represents an Object that holds vertices that can be rendered by OpenGL.
 */
class GameObject
{
public:
    /** Constructs a GameObject and attatches it to the world's physics.
     */
    GameObject(WorldPhysics & worldPhysics, Model* model) : physicsProperties (worldPhysics.getWorld())
    {

        // Come up with better default naming
		name = "Game Object";
        
        // By default an object is not renderable
		setRenderable(false);
		setModel(model);

		objType = GameObjectType::Generic;

		setScore(0);
		setLives(1);
		xVel = 0;
		yVel = 0;
		setActive(true);
		cappedMoveSpeed = 6;
		cappedJumpSpeed = 15;
		setAnimationSpeed(MED);
        physicsProperties.setIsStatic(true);
        updateOrigin();
		setScale(1.0, 1.0);
    }
    
    /** Copy Constructor - Used to easily make a copy of an existing GameObject
        (this is directly used by the WorldNavigator when alt-dragging)
     */
    GameObject (GameObject & objectToCopy, WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.getWorld())
    {
        this->name = objectToCopy.name;
        this->renderable = objectToCopy.renderable;
        this->renderableObject = objectToCopy.renderableObject;
		setMoveSpeed(objectToCopy.getMoveSpeed());
		setJumpSpeed(objectToCopy.getJumpSpeed());
        this->actionToAudio = objectToCopy.actionToAudio;
		this->objType = objectToCopy.objType;
        this->physicsProperties.setIsStatic((objectToCopy.getPhysicsProperties().getIsStatic()));
        
        // This seems odd? We want to set origin to wherever an object is placed
        // in edit mode.
        updateOrigin();
    }

	GameObject(WorldPhysics & worldPhysics, Model* model, ValueTree gameObjectValueTree) : physicsProperties(worldPhysics.getWorld())
	{

		setModel(model);
		cappedMoveSpeed = 6;
		cappedJumpSpeed = 15;
		score = 0;
		parseFrom(gameObjectValueTree);

	}

	virtual ~GameObject() {

	}

    String getName()
    {
        return name;
    }
    void setName(String name)
    {
        this->name = name;
    }
    
    // Rendering Data ==========================================================
	void setRenderable(bool canRender) {
		renderable = canRender;
	}
    bool isRenderable()
    {
        return renderable;
    }

    /** Sets if the renderable object should be rendered as being "Selected"
        in the GameView. This renders the object as highlighted in the
        GameView.
     */
    void setRenderableIsSelected(bool isSelected)
    {
		renderableObject.isSelected = isSelected;
    }
    
    /** Gets the renderable object for reading and copying.
		(const was taken out for now, put back in with getters/setters in GameObject)
     */
    RenderableObject & getRenderableObject()
    {
        return renderableObject;
    }
    
    /** Sets the Model for this GameObject. This also makes the object renderable
        since it now has a model to render.
     */
    void setModel (Model * model)
    {
        this->renderableObject.model = model;
        renderable = true;
    }

    /** Sets the 2D position of a GameObject in world coordinates, not worrying
        about updating the postition in the physical world
     */
    void setPosition (GLfloat x, GLfloat y)
    {
        // Set position vector
        renderableObject.position.x = x;
        renderableObject.position.y = y;
        
        // Modify model matrix to translate object to correct position
        renderableObject.modelMatrix[3][0] = x;
        renderableObject.modelMatrix[3][1] = y;
        
    }
    
    /** Sets the 2D position of a GameObject in world coordinates and in the
        physics world
     */
    void setPositionWithPhysics (GLfloat x, GLfloat y)
    {
        // Update visual object position
        renderableObject.position.x = x;
        renderableObject.position.y = y;
        
        // Modify model matrix to translate object to correct position
        renderableObject.modelMatrix[3][0] = x;
        renderableObject.modelMatrix[3][1] = y;
        
        // Update physical object position
        physicsProperties.setPosition (x, y);
        updateOrigin();
    }
    
    /** Offsets the 2D position of a GameObject in world coordinates and in the
        physics world
     */
    void offsetPositionWithPhysics (float xOffset, float yOffset)
    {
        // Update visual object position
        renderableObject.position.x += xOffset;
        renderableObject.position.y += yOffset;
        
        // Modify model matrix to translate object to correct position
        renderableObject.modelMatrix[3][0] += xOffset;
        renderableObject.modelMatrix[3][1] += yOffset;
        
        // Update physical object position
        physicsProperties.offsetPosition (xOffset, yOffset);
        updateOrigin();

    }
    
    glm::vec2 getPosition()
    {
        return renderableObject.position;
    }
    
    /** Scales the rendered Model.
       (Stored vertices in physics and same mesh)
     */
    void setScale (float x, float y)
    {
        renderableObject.modelMatrix[0][0] = x;
        renderableObject.modelMatrix[1][1] = y;
        
        // Update physics so it knows about the visual change in vertices
        physicsProperties.updateModelScale(renderableObject.model, x, y);
    }

	glm::vec2 getScale() 
	{
		return glm::vec2(renderableObject.modelMatrix[0][0], renderableObject.modelMatrix[1][1]);
	}

    PhysicsProperties & getPhysicsProperties()
    {
        return physicsProperties;
    }
    
    /** Specifies whether or not the GameObject is at a given position in the
        world.
     */
    bool isAtPosition(glm::vec2 position)
    {
        // Get half width and half height of object
        float halfWidth = renderableObject.model->getWidth() / 2.0f;
        float halfHeight = renderableObject.model->getHeight() / 2.0f;
        
        // If the position is within the bounds of the RenderableObject, the
        // object is at that position
        if (renderableObject.position.x - halfWidth <= position.x &&
            position.x <= renderableObject.position.x + halfWidth &&
            renderableObject.position.y - halfHeight <= position.y &&
            position.y <= renderableObject.position.y + halfHeight)
        {
            return true;
        }

        
        // Otherwise return false
        return false;
    }
    
    /** Specifies whether or not the GameObject is in the given range/
     */
    bool isInRange (float xMin, float xMax, float yMin, float yMax)
    {
        
        // First check if center of object is in range
        if (xMin <= renderableObject.position.x && renderableObject.position.x <= xMax &&
            yMin <= renderableObject.position.y && renderableObject.position.y <= yMax)
        {
            return true;
        }
        
        // If center is not in range, check the position to try to use one of
        // the other sides of the square to determine if it is inside
        // . . . 
        
        
        
        return false;
    }
    
    
    /* Provides a singular function that can be called in multiple places
	 * to make changes to the origin of the object based on where it is positioned
	 * in the editor
	*/
	void updateOrigin()
    {
		origin.x = getPhysicsProperties().GetPosition().x;
		origin.y = getPhysicsProperties().GetPosition().y;
	}
	glm::vec2 getOrigin()
    {
		return origin;
	}

	// Audio to Action Mapping =================================================
    /** Maps an audio file to play when a specific action happens to this object
        in the physical game world.
     */
    void mapAudioFileToPhysicalAction (File audioFile, PhysicalAction action)
    {
        actionToAudio[action] = audioFile;
    }

    /** Gets the audio to play when a specific PhysicalAction occurs in the game.
     */
	File * getAudioFileForAction(PhysicalAction action)
	{
		auto mapIterator = actionToAudio.find(action);
		if (mapIterator != actionToAudio.end())
			return &(actionToAudio.find(action)->second);
		else
			return nullptr;
	}

	//Numerical speeds for the object
	float getRunSpeedVelocity()
    {
		return xVel;
	}
	float getJumpSpeedVelocity()
    {
		return yVel;
	}
	void setMoveSpeed(Speed moveSpeed)
	{
		this->moveSpeed = moveSpeed;
		switch (this->moveSpeed) {
		case FAST:
			xVel = 3;
			break;
		case MED:
			xVel = 2;
			break;
		case SLOW:
			xVel = 1;
			break;
		}
	}
	void setJumpSpeed(Speed jumpspeed) {
		this->jumpSpeed = jumpspeed;
		switch (this->jumpSpeed) {
		case FAST:
			yVel = 10;
			break;
		case MED:
			yVel = 8;
			break;
		case SLOW:
			yVel = 6;
			break;
		}
	}

	//Enumerated Speed values for the object
	Speed getMoveSpeed() {
		return moveSpeed;
	}
	Speed getJumpSpeed() {
		return jumpSpeed;
	}
	GameObjectType getObjType()
	{
		return objType;
	}
	void setObjType(GameObjectType type)
	{
		objType = type;
	}
	// Player Lives 
	int getLives()
    {
		return lives;
	}
	void setLives(int newLives)
    {
		lives = newLives;
		currLives = newLives;
	}
	int getCurrLives() {
		return currLives;
	}
	void setCurrLives(int newLives) {
		currLives = newLives;
	}

	int getScore()
	{
		return score;
	}
	int getCurrScore() {
		return currScore;
	}
	void setScore(int newScore)
	{
		score = newScore;
		currScore = newScore;
	}

	void addCurrScore(int points) {
		currScore += points;
	}
	void addScore(int points) {
		score += points;
	}
	
	//active State of object
	bool getIsActive() {
		return isActive;
	}
	void setActive(bool active) {
		isActive = active;
		physicsProperties.setActiveStatus(active);
	}

	// Animation speed
	Speed getAnimationSpeed() {
		return renderableObject.animationProperties.getAnimationSpeed();
	}
	void setAnimationSpeed(Speed animSpeed) {
		renderableObject.animationProperties.setAnimationSpeed(animSpeed);
	}

	void parseFrom(ValueTree valueTree) {

		name = valueTree.getProperty(Identifier("name"));
		int objectTypeInt = valueTree.getProperty(Identifier("type"));
		
		switch (objectTypeInt) {

		case 0:
			objType = Generic;
			break;
		case 1:
			objType = Player;
			break;
		case 2:
			objType = Enemy;
			break;
		case 3:
			objType = Collectable;
			break;
		case 4:
			objType = Checkpoint;
			break;
		case 5:
			objType = Bounds;
			break;
		}

		ValueTree moveSpeedTree = valueTree.getChildWithName(Identifier("MoveSpeed"));
		int moveSpeedInt = moveSpeedTree.getProperty(Identifier("value"));

		switch (moveSpeedInt) {

		case 0:
			setMoveSpeed(SLOW);
			break;
		case 1:
			setMoveSpeed(MED);
			break;
		case 2:
			setMoveSpeed(FAST);
			break;
		}

		ValueTree jumpSpeedTree = valueTree.getChildWithName(Identifier("JumpSpeed"));
		int jumpSpeedInt = jumpSpeedTree.getProperty(Identifier("value"));

		switch (jumpSpeedInt) {

		case 0:
			setJumpSpeed(SLOW);
			break;
		case 1:
			setJumpSpeed(MED);
			break;
		case 2:
			setJumpSpeed(FAST);
			break;
		}

		ValueTree renderableTree = valueTree.getChildWithName(Identifier("Renderable"));
		renderable = renderableTree.getProperty(Identifier("value"));
		if (!renderable) {
			setActive(false);
		}
		ValueTree originTree = valueTree.getChildWithName(Identifier("Origin"));
		origin.x = originTree.getProperty(Identifier("x"));
		origin.y = originTree.getProperty(Identifier("y"));
		setPositionWithPhysics(origin.x, origin.y);

		ValueTree livesTree = valueTree.getChildWithName(Identifier("Lives"));
		lives = livesTree.getProperty(Identifier("value"));
		currLives = lives;
		renderableObject.parseFrom(valueTree.getChildWithName(Identifier("RenderableObject")));

		physicsProperties.parseFrom(valueTree.getChildWithName(Identifier("PhysicsProperties")));

		glm::vec2 scale;
		setScore(0);
		ValueTree scaleTree = valueTree.getChildWithName(Identifier("Scale"));
		scale.x = scaleTree.getProperty(Identifier("x"));
		scale.y = scaleTree.getProperty(Identifier("y"));
		setScale(scale.x, scale.y);

		ValueTree audioActionsValueTree = valueTree.getChildWithName(Identifier("ActionToAudio"));


		for (ValueTree audioActionTree : audioActionsValueTree) {
			int physicalActionInt = audioActionTree.getProperty(Identifier("action"));

			switch (physicalActionInt) {

			case 0:
				actionToAudio[collsion] = File(File::getCurrentWorkingDirectory().getFullPathName() + "/" + audioActionTree.getProperty(Identifier("file")).toString());
				break;
			case 1:
				actionToAudio[inRange] = File(File::getCurrentWorkingDirectory().getFullPathName() + "/" + audioActionTree.getProperty(Identifier("file")).toString());
				break;
			case 2:
				
				actionToAudio[death] = File(File::getCurrentWorkingDirectory().getFullPathName() + "/" + audioActionTree.getProperty(Identifier("file")).toString());
				break;
			}
			
		}
	}

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the game
		ValueTree gameObjectSerialization("GameObject");

		gameObjectSerialization.setProperty(Identifier("name"), var(this->getName()), nullptr);

		//Serialize game object type
		int objectTypeInt;

		switch (objType) {

		case Generic:
			objectTypeInt = 0;
			break;
		case Player:
			objectTypeInt = 1;
			break;
		case Enemy:
			objectTypeInt = 2;
			break;
		case Collectable:
			objectTypeInt = 3;
			break;
		case Checkpoint:
			objectTypeInt = 4;
			break;
		}

		gameObjectSerialization.setProperty(Identifier("type"), var(objectTypeInt), nullptr);

		//Serialize Renderable bool
		ValueTree isRenderableValueTree("Renderable");

		isRenderableValueTree.setProperty(Identifier("value"), var(renderable), nullptr);

		gameObjectSerialization.addChild(isRenderableValueTree, -1, nullptr);

		//Serialize Renderable Object
		gameObjectSerialization.addChild(renderableObject.serializeToValueTree(), -1, nullptr);

		gameObjectSerialization.addChild(physicsProperties.serializeToValueTree(), -1, nullptr);

		ValueTree moveSpeedTree("MoveSpeed");

		int moveSpeedInt;

		switch (moveSpeed) {

		case SLOW:
			moveSpeedInt = 0;
			break;
		case MED:
			moveSpeedInt = 1;
			break;
		case FAST:
			moveSpeedInt = 2;
			break;
		}

		moveSpeedTree.setProperty(Identifier("value"), var(moveSpeedInt), nullptr);
		gameObjectSerialization.addChild(moveSpeedTree, -1, nullptr);

		ValueTree jumpSpeedTree("JumpSpeed");

		int jumpSpeedInt;

		switch (jumpSpeed) {

		case SLOW:
			jumpSpeedInt = 0;
			break;
		case MED:
			jumpSpeedInt = 1;
			break;
		case FAST:
			jumpSpeedInt = 2;
			break;
		}

		jumpSpeedTree.setProperty(Identifier("value"), var(jumpSpeedInt), nullptr);
		gameObjectSerialization.addChild(jumpSpeedTree, -1, nullptr);

		ValueTree originTree("Origin");
		originTree.setProperty(Identifier("x"), var(origin.x), nullptr);
		originTree.setProperty(Identifier("y"), var(origin.y), nullptr);
		gameObjectSerialization.addChild(originTree, -1, nullptr);

		glm::vec2 scale = getScale();

		ValueTree scaleTree("Scale");
		scaleTree.setProperty(Identifier("x"), var(scale.x), nullptr);
		scaleTree.setProperty(Identifier("y"), var(scale.y), nullptr);
		gameObjectSerialization.addChild(scaleTree, -1, nullptr);

		ValueTree livesTree("Lives");
		livesTree.setProperty(Identifier("value"), var(lives), nullptr);
		gameObjectSerialization.addChild(livesTree, -1, nullptr);

		ValueTree actionToAudioTree = ValueTree("ActionToAudio");


		for (std::map<PhysicalAction,File>::iterator it = actionToAudio.begin(); it != actionToAudio.end(); ++it)
		{
			ValueTree audioActionTree = ValueTree("AudioAction");
			audioActionTree.setProperty(Identifier("file"), var(it->second.getRelativePathFrom(File::getCurrentWorkingDirectory())), nullptr);

			int physicalActionInt;

			switch (it->first) {

			case collsion:
				physicalActionInt = 0;
				break;
			case inRange:
				physicalActionInt = 1;
				break;
			case death:
				physicalActionInt = 2;
				break;
			}

			audioActionTree.setProperty(Identifier("action"), var(physicalActionInt), nullptr);

			actionToAudioTree.addChild(audioActionTree, -1, nullptr);

		}

		gameObjectSerialization.addChild(actionToAudioTree, -1, nullptr);
		

		return gameObjectSerialization;
	}

protected:
	GameObjectType objType;
	float cappedMoveSpeed, cappedJumpSpeed;
private:
	
    /** Name of object */
    String name;
	int lives, score;
	int currLives, currScore;
	glm::vec2 origin;
    /** Specifies wether or not the object will be rendered visually to the screen */
    bool renderable, isActive;
    /** 
		Renderable representation of this object.
     */
    RenderableObject renderableObject;


    /** Physical properties associated with the object */
    PhysicsProperties physicsProperties;
	float xVel, yVel;
	// Speed of object jump/run
	Speed moveSpeed, jumpSpeed;
    /** Map of in-game physics-based actions to specific audio files */
    std::map<PhysicalAction, File> actionToAudio;

	JUCE_LEAK_DETECTOR(GameObject)
};
