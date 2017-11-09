#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "glm/glm.hpp"
#include <map>

/** Represents a single renderable frame that is send to GameView to render.
    It includes all data needed to render a frame in OpenGL.
 */
class RenderSwapFrame
{
public:
    
    void setRenderableObjects (vector<RenderableObject> renderableObjects)
    {
        this->renderableObjects = renderableObjects;
    }
    
	//Const removed for now to allow accessing of animationProperties in RenderableObject
    vector<RenderableObject> & getRenderableObjects()
    {
        return renderableObjects;
    }
    
    void setViewMatrix (glm::mat4 viewMatrix)
    {
        this->viewMatrix = viewMatrix;
    }
    
    const glm::mat4 & getViewMatrix()
    {
        return viewMatrix;
    }

	void setAttribute(String key, var value) {
		attrs[key] = value;
	}

	var* getAttribute(String key) {
		std::map<String, var>::iterator it;

		it = attrs.find(key);
		if (it != attrs.end()) {
			return &attrs[key];
		}

		return nullptr;
	}

private:
    vector<RenderableObject> renderableObjects;
    glm::mat4 viewMatrix;
	map<String, var> attrs;
    
	JUCE_LEAK_DETECTOR(RenderSwapFrame)
};

