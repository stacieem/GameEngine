//
//  GameHUD.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** Renders a Heads Up Display with a transparent background.
 */
class GameHUD : public Component
{
public:

    GameHUD()
    {
        addAndMakeVisible(slider);
        setOpaque(false);
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
        slider.setBounds(20, 20, 200, 50);
    }
    
private:
    Slider slider;
    
    
};
