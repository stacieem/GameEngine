#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ScoreHUD : public Component
{

public:

	ScoreHUD()
	{
		score = 0;
		addAndMakeVisible(scoreLabel);
		scoreLabel.setText("Score: " + String(score), dontSendNotification);
		scoreLabel.setJustificationType(Justification::left);
		scoreLabel.setFont(Font(28.0f));

		

	}

	void paint(Graphics &g) override
	{
		

	}

	void resized() override
	{
		scoreLabel.setBounds(getLocalBounds());
	}


	void setScore(int score) {

		if (score != this->score) {
			this->score = score;
			scoreLabel.setText("Score: " + String(score), dontSendNotification);
			repaint();
		}
		
	}


private:
	Label scoreLabel;
	int score;

};

