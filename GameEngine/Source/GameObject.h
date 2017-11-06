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

/** Represents an Object that holds vertices that can be rendered by OpenGL.
 */
class GameObject
{
public:
    /** Constructs a GameObject and attatches it to the world's physics.
     */
    GameObject(WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.getWorld())
    {

        // Come up with better default naming
		name = "Game Object";
        
        // By default an object is not renderable
        renderable = false;
        
		objType = GameObjectType::Generic;
		lives = 0;
		xVel = 0;
		yVel = 0;
		cappedMoveSpeed = 10;
		cappedJumpSpeed = 30;
		setAnimationSpeed(MED);
        physicsProperties.setIsStatic(true);
        updateOrigin();
    }
    
    /** Copy Constructor - Used to easily make a copy of an existing GameObject
        (this is directly used by the WorldNavigator when alt-dragging)
     */
    GameObject (GameObject & objectToCopy, WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.getWorld())
    {
        this->name = objectToCopy.name;
        this->renderable = objectToCopy.renderable;
        this->renderableObject = objectToCopy.renderableObject;
        this->xVel = objectToCopy.xVel;
        this->yVel = objectToCopy.yVel;
        this->actionToAudio = objectToCopy.actionToAudio;
        
        this->physicsProperties.setIsStatic((objectToCopy.getPhysicsProperties().getIsStatic()));
        
        // This seems odd? We want to set origin to wherever an object is placed
        // in edit mode.
        updateOrigin();
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
    
    /** Scales the rendered Model.
        PROBLEM: Physics still only knows the original vertices, not the scaled
        vertices, we must give the physics world an update about the scale of
        this object
     */
    void setScale (float x, float y)
    {
        renderableObject.modelMatrix[0][0] = x;
        renderableObject.modelMatrix[1][1] = y;
        
        // Update physics so it knows about the visual change in vertices
        physicsProperties.updateModelScale(renderableObject.model, x, y);
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
        actionToAudio.insert(std::pair<PhysicalAction, File>(action, audioFile));
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
			yVel = 12;
			break;
		case MED:
			yVel = 10;
			break;
		case SLOW:
			yVel = 7;
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

	// Player Lives 
	int getLives()
    {
		return lives;
	}
	void setLives(int newLives)
    {
		lives = newLives;
	}

	// Animation speed
	Speed getAnimationSpeed() {
		return renderableObject.animationProperties.getAnimationSpeed();
	}
	void setAnimationSpeed(Speed animSpeed) {
		this->animSpeed = animSpeed;
		renderableObject.animationProperties.setAnimationSpeed(animSpeed);
	}

protected:
	GameObjectType objType;
	float cappedMoveSpeed, cappedJumpSpeed;
private:
	
    /** Name of object */
    String name;
	int lives;
	glm::vec2 origin;
    /** Specifies wether or not the object will be rendered visually to the screen */
    bool renderable;
    /** 
		Renderable representation of this object.
     */
    RenderableObject renderableObject;


    /** Physical properties associated with the object */
    PhysicsProperties physicsProperties;
	float xVel, yVel;
	// Speed of object jump/run
	Speed moveSpeed, jumpSpeed, animSpeed;
    /** Map of in-game physics-based actions to specific audio files */
    std::map<PhysicalAction, File> actionToAudio;

	JUCE_LEAK_DETECTOR(GameObject)
};
