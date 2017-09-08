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

/** Renders a Heads Up Display with a transparent background.
 */
class GameHUD : public Component
{
public:

    GameHUD()
    {
        addAndMakeVisible(healthBar);
        healthBar.setRange(0, 10, 1.0);
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
        const int w = getWidth();
        const int h = getHeight();
        
        const int healthBarW = w / 14;
        const int healthBarH = h * 0.9;
        const int margin = 20;
        
        healthBar.setBounds(margin, (h - healthBarH) / 2, healthBarW, healthBarH);
    }
    
private:
    HealthBar healthBar;
    
    
};
