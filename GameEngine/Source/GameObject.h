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

		yVelocityCap = 0;
		xVelocityCap = 0;
		xVel = 0;
		yVel = 0;



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
   
	enum ObjectStateType {
		DYNAMIC,
		STATIC
	};


    bool isRenderable()
    {
        return renderable;
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
    }

	/** Sets the 2D position of a GameObject in world coordinates and in the
	physics world
	*/
	void setYPositionWithPhysics(GLfloat y)
	{
		// Update visual object position
		renderableObject.position.y = y;

		// Modify model matrix to translate object to correct position
		renderableObject.modelMatrix[3][1] = y;

		// Update physical object position
		physicsProperties.setPosition(renderableObject.position.x, y);
	}

	/** Sets the 2D position of a GameObject in world coordinates and in the
	physics world
	*/
	void setXPositionWithPhysics(GLfloat x)
	{
		// Update visual object position
		renderableObject.position.x = x;

		// Modify model matrix to translate object to correct position
		renderableObject.modelMatrix[3][0] = x;

		// Update physical object position
		physicsProperties.setPosition(x, renderableObject.position.y);
	}

    PhysicsProperties & getPhysicsProperties()
    {
        return physicsProperties;
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
 
    // Animation ?? ============================================================

	float getXVel()
    {
		return xVel;
	}

	
	float getYVel() {

		return yVel;
	}
	
	float getXVelocityCap() {
		return xVelocityCap;
	}
	
	float getYVelocityCap() {
		return yVelocityCap;
	}
	
	GameObjectType getObjType() {
		return objType;
	}



	void setXVelocityCap(Speed moveSpeed) {
		int newXVel = 0;
		switch (moveSpeed) {
		case FAST:
			newXVel = 8;
			break;
		case MED:
			newXVel = 5;
			break;
		case SLOW:
			newXVel = 2;
			break;
		}

		xVelocityCap = newXVel;
		if (xVel > xVelocityCap) {
			xVel = xVelocityCap;
		}
		xVel = xVelocityCap / 3;
		
	}

	void setYVelocityCap(Speed jumpspeed) {
		int newYVel = 0;
		switch (jumpspeed) {
		case FAST:
			newYVel = 40;
			break;
		case MED:
			newYVel = 30;
			break;
		case SLOW:
			newYVel = 20;
			break;
		}
		yVelocityCap = newYVel;
		if (yVel > yVelocityCap) {
			yVel = yVelocityCap;
		}

		yVel = yVelocityCap / 3;
	}


	void setXVel(float newXVel)
    {
		xVel = newXVel;
		if (xVel > xVelocityCap) {
			xVel = xVelocityCap;
		}
	}

	void setYVel(float newYVel) {
		yVel = newYVel;
		if (yVel > yVelocityCap) {
			yVel = yVelocityCap;
		}
	}

	void updateState(ObjectStateType state) {
		switch (state) {
		case STATIC:
			physicsProperties.toStatic();
			break;
		case DYNAMIC:
			physicsProperties.toDynamic();
			break;
		}
	}

protected:
	GameObjectType objType;

private:
	
    /** Name of object */
    String name;
    
    /** Specifies wether or not the object will be rendered visually to the screen */
    bool renderable;
    
    /** Renderable representation of this object.
     */
    RenderableObject renderableObject;
    
    /** Physical properties associated with the object */
    PhysicsProperties physicsProperties;

	//Physics related accelerations and max velocities
	/*EVENTUALLY should go in PhysicsProperties*/
	GLfloat xVel, yVel;
	float xVelocityCap, yVelocityCap;
    
    /** Map of in-game physics-based actions to specific audio files */
    std::map<PhysicalAction, File> actionToAudio;
    
	String objName;

	JUCE_LEAK_DETECTOR(GameObject)
};
