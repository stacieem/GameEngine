/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    button.setButtonText ("Lolz wutt");
    addAndMakeVisible (button);
    
    backgroundColour = Colours::black;
    button.addListener (this);
    
    setSize (600, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll (backgroundColour);

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    button.setBounds (30, 30, 200, 200);
}

void MainContentComponent::buttonClicked (Button * buttonPtr)
{
    if (buttonPtr == &button)
    {
		if (backgroundColour == Colours::black) {
			backgroundColour = Colours::white;
		}
		else {
			backgroundColour = Colours::black;
		}
        repaint();
    }
}
