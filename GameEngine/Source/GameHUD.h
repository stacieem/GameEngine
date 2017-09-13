//
//  GameHUD.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "HealthBar.h"
#include "GameObject.h"

/** Renders a Heads Up Display with a transparent background.
 */
class GameHUD : public Component, public Button::Listener, public ChangeListener
{
public:

    GameHUD()
    {
        setOpaque(false);
        
//        addAndMakeVisible(healthBar);
//        healthBar.setRange(0, 10, 1.0);
        
        resetButton.addListener(this);
        
        resetButton.setButtonText("Reset");
        addAndMakeVisible(resetButton);
        
        heightSlider.setRange (0, 1.0f);
        heightSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        addAndMakeVisible(heightSlider);
    }
    
    ~GameHUD()
    {
        
    }
    
    
    // JUCE Callbacks ==========================================================
    
    void paint (Graphics &g) override
    {
    }
    
    void resized () override
    {
        const int w = getWidth();
        const int h = getHeight();
        
        const int healthBarW = w / 12;
        const int healthBarH = h * 0.9;
        const int margin = 20;
        
        //healthBar.setBounds(margin, (h - healthBarH) / 2, healthBarW, healthBarH);
        resetButton.setBounds (w - 120, margin, 100, 50);
        heightSlider.setBounds(margin, (h - healthBarH) / 2, healthBarW, healthBarH);
    }
    
    // Crazy hacky thing that should not be done for the actual engine wow.
    void buttonClicked (Button* button) override
    {
        if (button == &resetButton)
        {
            (*gameObjects)[0]->getPhysicsProperties().setImpulse(0, 8);
        }
    }
    
    void takeThisMagicalPointer (OwnedArray<GameObject> & gameObjects, WorldPhysics & wrld)
    {
        this->gameObjects = &gameObjects;
        this->wrld = &wrld;
    }
    
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
        heightSlider.setValue(*MAX_HEIGHT);
    }
    
    void setMaxHeightPointer (GLfloat * ht)
    {
        MAX_HEIGHT = ht;
    }
    
    
private:
    //HealthBar healthBar;
    TextButton resetButton;
    Slider heightSlider;

    // My magic evil pointers that need to be taken away
    OwnedArray<GameObject> * gameObjects;
    WorldPhysics * wrld;
    
    GLfloat * MAX_HEIGHT;
    
    
};
