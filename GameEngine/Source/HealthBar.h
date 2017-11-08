//
//  HealthBar.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/29/17.
//
//
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class HealthBar : public Component, public ChangeListener
{
    
public:
    
    HealthBar()
    {
		addAndMakeVisible(lifeLabel);
		lifeLabel.setText("- LIFE -", dontSendNotification);
		lifeLabel.setJustificationType(Justification::centredTop);
		lifeLabel.setFont(Font(14.0f));

		File f = File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/heart.png");

		img = ImageFileFormat::loadFrom(f);
		img = img.rescaled(30, 30);

		
    }

	void paint(Graphics &g) override
	{
		g.drawImageAt(img, 0, 20);
		g.drawImageAt(img, 35, 20);
		g.drawImageAt(img, 70, 20);
		g.drawImageAt(img, 105, 20);
		g.drawImageAt(img, 140, 20);
	}

    
    void resized() override
    {
		lifeLabel.setBounds(getLocalBounds());
    }
    
    void setChangeBroadcaster (ChangeBroadcaster * changeBroadcaster)
    {
        healthChangeBroadcaster = changeBroadcaster;
    }
    
    void changeListenerCallback (ChangeBroadcaster *source) override
    {
        // If the data is changed, pull the data
        if (source == healthChangeBroadcaster)
        {
            
        }
    }


private:
    ChangeBroadcaster * healthChangeBroadcaster;
	Label lifeLabel;
	Image img;

    
};

