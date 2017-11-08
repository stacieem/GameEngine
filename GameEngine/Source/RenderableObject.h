//
//  RenderableObject.h
//  GameEngine
//
//  Created by Tim Arterbury on 10/20/17.
//
//

#pragma once
#include "Model.h"
#include "glm/glm.hpp"
#include "AnimationProperties.h"

struct RenderableObject
{
    RenderableObject()
    {
        // No default model
        model = nullptr;
        
        // Default position
        position = glm::vec4(glm::vec3(0.0), 1.0);
        
        // Default model matrix
        modelMatrix = glm::mat4(1.0);
        
        // Default should not be selected for rendering
        isSelected = false;
    }
    
    /** Visual representation of an object. Points to a model resource since
     multiple RenderableObjects may reference the same Model.
     */
    Model * model; // Includes vertices, textures, etc.
    
    /** Transformations applied to this particular object model */
    glm::mat4 modelMatrix;
    
    /** Position of model in world space */
    glm::vec4 position; // This is probably unneded since this is stored in the
    // model matrix

	//Stores textures for the animation, maybe eventually move them to the model
	AnimationProperties animationProperties;

	File renderTexture;

    /** Specifies whether this renderable object is selected in the rendered GameView */
    bool isSelected;
    
};

