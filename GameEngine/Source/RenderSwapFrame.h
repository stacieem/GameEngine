#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "glm/glm.hpp"

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
    
    const vector<RenderableObject> & getRenderableObjects()
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

private:
    vector<RenderableObject> renderableObjects;
    glm::mat4 viewMatrix;
    
	JUCE_LEAK_DETECTOR(RenderSwapFrame)
};

