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
    

		//Set default texture
		//setTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/flower.jpg"));
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
    
    /** Returns true of the object is renderable and false otherwise.
     */
    bool isRenderable()
    {
        return renderable;
    }
    
    /** Gets the renderable object for reading and copying.
     */
    const RenderableObject & getRenderableObject()
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
        
        // Maybe update the stored physics properties position
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


    // TO BE REMOVED
    // Textures ================================================================
	/**
	* Set the name of the texture to use for this object. Currently, 
	* all textures are loaded at runtime in GameView
	*/
	void setTexture(File tex)
    {
		textureFile = tex;
	}

	File getTexture()
    {
		return textureFile;
	}
    

    // Animation ?? ============================================================

	float getXVel()
    {
		return xVel;
	}
	float getYVel()
    {
		return yVel;
	}


	void setXVel(float newXVel)
    {
		xVel = newXVel;
	}
    
	void setYVel(float newYVel)
    {
		yVel = newYVel;
	}
    
private:
    
    // Physical Position =======================================================

    // FIX
    // What are these? Are these physical properties? If so it should be in
    // Physical Properties. Else is it an animation thing? Consider putting into
    // an Animatoin struct or class.
	GLfloat xVel, yVel;

    /** Name of object */
    String name;
    
    /** Specifies wether or not the object will be rendered visually to the screen */
    bool renderable;
    
    /** Renderable representation of this object.
     */
    RenderableObject renderableObject;
    
    /** Physical properties associated with the object */
    PhysicsProperties physicsProperties;
    
    /** Map of in-game physics-based actions to specific audio files */
    std::map<PhysicalAction, File> actionToAudio;
    
    
    // THIS WILL BE REMOVED AND IS INCLUDED IN THE MODEL OBJECT
	File textureFile;

    
//    AnimationList animations;
//    std::map<> actionToAnimationMap;
    
    // Someone setting up a peice of audio to be triggered by an action:
    // actionToAudioMap.map(files[0], IN_RANGE_ACTION);
    // actionToAudioMap.map(files[1], COLLISION_ACTION);
    // actionToAudioMap.map(files[1], INTERACTION_ACTION);
    //
    // actionToAnimationMap.map (animations[0], IN_RANGE_ACTION);
    //
    // Then the corresponding audio is called in the actionTriggered calback in
    //  this object:
    //
    //  void actionTriggered (actionId) {
    //
    //      playAudio (actionToAudioMap.get(actionId));
    //      startAnimaction (actionToAnimationMap.get(actionId));
    //      // etc for all other things to do when an action is triggered . . .
    // }
    
	JUCE_LEAK_DETECTOR(GameObject)
};
