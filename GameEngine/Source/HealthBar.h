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
        //addAndMakeVisible (slider);
        slider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    }

    void setValue (double newValue)
    {
        slider.setValue (newValue, juce::NotificationType::sendNotificationAsync);

    }
    
    void setRange (double newMinimum, double newMaximum, double newInterval=0)
    {
        slider.setRange (newMinimum, newMaximum, newInterval);
    }
    
    void resized() override
    {
        slider.setBounds(getLocalBounds());
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
    Slider slider;
    ChangeBroadcaster * healthChangeBroadcaster;
    
    
};

