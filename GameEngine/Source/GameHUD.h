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
        
        addAndMakeVisible(healthBar);
        healthBar.setRange(0, 10, 1.0);
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
        
        healthBar.setBounds(margin, (h - healthBarH) / 2, healthBarW, healthBarH);
    }
    
    /** Consider getting rid of this ...
        we must hash out a messageing system.
        Maybe we register some MessageCenter that registers a bunch of listeners
        like a button and maps out GameCommands to do based on those.
     */
    void buttonClicked (Button* button) override
    {
    }
    
    /** Consider getting rid of this ... 
        we must hash out a messageing system.
     */
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
    
    }
    
    
private:
    HealthBar healthBar;
};
