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

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the renderable object
		ValueTree renderableObjectSerialization = ValueTree("RenderableObject");

		//Serialize position
		ValueTree positionMatrixValueTree = ValueTree("Position");

		for (int i = 0; i < 4; i++) {
			

			ValueTree positionElement = ValueTree("PositionElement");

			positionElement.setProperty(Identifier("value"), var((double)position[i]), nullptr);
			positionMatrixValueTree.addChild(positionElement, -1, nullptr);

		
		}

		renderableObjectSerialization.addChild(positionMatrixValueTree, -1, nullptr);

		//Serialize model matrix
		ValueTree modelMatrixValueTree = ValueTree("ModelMatrix");

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {

				ValueTree matrixElement = ValueTree("MatrixElement");

				matrixElement.setProperty(Identifier("value"), var((double)modelMatrix[i][j]), nullptr);
				modelMatrixValueTree.addChild(matrixElement, -1, nullptr);

			}
		}

		renderableObjectSerialization.addChild(modelMatrixValueTree, -1, nullptr);

		//Serialize animation properties
		renderableObjectSerialization.addChild(animationProperties.serializeToValueTree(), -1, nullptr);

		return renderableObjectSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		ValueTree viewMatrixValueTree = valueTree.getChildWithName(Identifier("ModelMatrix"));

		Array<double> vals;

		for (ValueTree vt : viewMatrixValueTree) {
			double val = vt.getProperty(Identifier("value"));
			vals.add(val);
		}

		modelMatrix = glm::mat4(vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7], vals[8], vals[9], vals[10], vals[11], vals[12], vals[13], vals[14], vals[15]);


		ValueTree positionValueTree = valueTree.getChildWithName(Identifier("Position"));

		Array<double> pos;

		for (ValueTree vt : positionValueTree) {
			double val = vt.getProperty(Identifier("value"));
			pos.add(val);
		}

		position = glm::vec4(pos[0], pos[1], pos[2], pos[3]);

		//Animation properties
		animationProperties.parseFrom(valueTree.getChildWithName(Identifier("AnimationProperties")));


	}

    /** Specifies whether this renderable object is selected in the rendered GameView */
    bool isSelected;
    

};

