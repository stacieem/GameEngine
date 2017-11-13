//
//  HealthBar.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/29/17.
//
//
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class HealthBar : public Component
{
    
public:
    
    HealthBar()
    {
		lives = 1;
		addAndMakeVisible(lifeLabel);
		lifeLabel.setText("- LIFE -", dontSendNotification);
		lifeLabel.setJustificationType(Justification::centredTop);
		lifeLabel.setFont(Font(14.0f));

		addAndMakeVisible(multiplierLabel);
		multiplierLabel.setText("x", dontSendNotification);
		multiplierLabel.setJustificationType(Justification::right);
		multiplierLabel.setFont(Font(28.0f));
		 
		File f = File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/heart.png");

		img = ImageFileFormat::loadFrom(f);
		img = img.rescaled(30, 30);
    }

	void paint(Graphics &g) override
	{
		if (lives > 5) {
			multiplierLabel.setText("x" + String(lives), dontSendNotification);

			g.drawImageAt(img, 70, 20);
			multiplierLabel.setVisible(true);
			return;
		}

		multiplierLabel.setVisible(false);
		int x = 0;
		for (int i = 0; i < lives; i++) {
			g.drawImageAt(img, x, 20);
			x += 35;
		}
		
		
	}

    
    void resized() override
    {
		lifeLabel.setBounds(getLocalBounds());
		multiplierLabel.setBounds(105, 20, 40, 30);
   
    }

	void setLives(int lives) {
		this->lives = lives;
	}

	void setLifeTexture(File file) {
		if (texFile == file) {
			return;
		}
		texFile = file;
		img = ImageFileFormat::loadFrom(file);
		img = img.rescaled(30, 30);
		
	}


private:
	Label lifeLabel;
	Label multiplierLabel;
	Image img;
	File texFile;
	int lives;
    
};

