//
//  GameView.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameHUD.h"

/** Represents the view of any game being rendered.
    It includes an OpenGL Renderer to render either 2D or 3D graphics and a
    GameHUD component to render elements over the top of the OpenGL Renderer.
 */
class GameView :    public Component,
                    private OpenGLRenderer
{
    
public:
    GameView()
    {
        openGLContext.setRenderer(this);
        openGLContext.attachTo(*this);
        
        addAndMakeVisible(gameHUD);
        
        isEnabled = false;
        setOpaque(true);
    }
    
    ~GameView()
    {
        openGLContext.detach();
    }
    
    /** Enables or disables the OpenGL layer of GameView. Enabling continuously
        renders and repaints with OpenGL. Disabling will set a solid colour fill
        for the component and the component will do no processing.
     */
    void setOpenGLEnabled (bool enabled)
    {
        if (enabled)
        {
            openGLContext.setContinuousRepainting(true);
            isEnabled = true;
        }
        else
        {
            openGLContext.setContinuousRepainting(false);
            isEnabled = false;
        }
    }
    
    
    // OpenGL Callbacks ========================================================
    void newOpenGLContextCreated() override
    {
        
    }
    
    void openGLContextClosing() override
    {
        
    }
    
    void renderOpenGL() override
    {
        jassert (OpenGLHelpers::isContextActive());
        
        OpenGLHelpers::clear (Colours::red);
    }
    
    
    // JUCE Callbacks ==========================================================
    void resized() override
    {
        gameHUD.setBounds(getLocalBounds());
    }
    
    void paint(Graphics & g) override
    {}
    
    
private:
    
    bool isEnabled;
    
    // OpenGL Rendering
    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shader; // When we use shaders to make crap look dope
    
    // Components
    GameHUD gameHUD;
    
};
