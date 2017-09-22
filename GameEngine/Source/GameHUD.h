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
class GameHUD : public Component,  public ChangeListener
{
public:

    GameHUD()
    {
        setOpaque(false);
        
        // Setup frame rate label
        addAndMakeVisible (frameRateLabel);
        frameRateLabel.setJustificationType (Justification::topLeft);
        frameRateLabel.setFont (Font (14.0f));
        frameRateLabel.setText("0 fps", dontSendNotification);
        
        //addAndMakeVisible(healthBar);
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
        
        frameRateLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75).translated(0.0, 20.0));
        //healthBar.setBounds(margin, (h - healthBarH) / 2, healthBarW, healthBarH);
    }

    /** Consider getting rid of this ... 
        we must hash out a messageing system.
     */
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
    
    }
    
    void setFrameRate (int frameRate)
    {
        frameRateLabel.setText(String(frameRate) + " fps", dontSendNotification);
        repaint();
    }
    
    
private:
    Label frameRateLabel;
    HealthBar healthBar;
};
