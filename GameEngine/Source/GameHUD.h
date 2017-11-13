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
#include "ScoreHUD.h"

/** Renders a Heads Up Display with a transparent background.
 */
class GameHUD : public Component,  public ChangeListener
{
public:

    GameHUD()
    {
        setOpaque(false);
        
        addAndMakeVisible(healthBar);
		addAndMakeVisible(scoreHUD);
		// Setup frame rate label
		//addAndMakeVisible(frameRateLabel);

		frameRateLabel.setJustificationType(Justification::topRight);
		frameRateLabel.setFont(Font(14.0f));
		frameRateLabel.setText("0 fps", dontSendNotification);


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
        const int healthBarH = h * 0.3;
        const int margin = 20;
        
        healthBar.setBounds(10, 10, 175, 75);
		scoreHUD.setBounds(w - 150, 10, 150, 75);
		frameRateLabel.setBounds(getLocalBounds().reduced(4).removeFromTop(75).translated(0.0, 20.0));
    }

    /** Consider getting rid of this ... 
        we must hash out a messageing system.
     */
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
		
    }
    
	void setFrameRate(int frameRate)
	{
		frameRateLabel.setText(String(frameRate) + " fps", dontSendNotification);
		repaint();
	}

	void setAttributes(map<String,var> attrs) {
		healthBar.setLives(attrs["lives"]);
		scoreHUD.setScore(attrs["score"]);
	}

    
private:
    HealthBar healthBar;
	ScoreHUD scoreHUD;

	Label frameRateLabel;
};
