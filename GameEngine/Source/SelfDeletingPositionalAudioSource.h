//
//  SelfDeletingAudioSouce.h
//  GameEngine
//
//  Created by Tim Arterbury on 10/3/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** Wrapper for a PositionalAudioSource that will delete itself and remove
    itself from a MixerAudioSource. The PositionableAudioSource source itself
    is not deleted, only the wrapper. This assumes that when you created a
    MixerAudioSource, you specified it would delete any of its AudioSources
    that are removed as an input source from the MixerAudioSource.
 */
class SelfDeletingPositionalAudioSource : public Timer
{
    
public:
    SelfDeletingPositionalAudioSource (PositionableAudioSource * audioSource, MixerAudioSource * mixerToRemoveFrom)
    {
        this->audioSource = audioSource;
        this->mixerToRemoveFrom = mixerToRemoveFrom;
        
        // Run garbage collection at constant interval
        this->startTimer(IS_FINISHED_CHECKING_INTERVAL_MILISECONDS);

    }
    
    ~SelfDeletingPositionalAudioSource()
    {
        this->stopTimer();
    }
    
    
    
    void timerCallback() override
    {
        if (!audioSource->isLooping())
        {
            if (audioSource->getNextReadPosition() > audioSource->getTotalLength())
            {
                // Remove audio source from MixerAudioSource (this will delete the audio source)
                mixerToRemoveFrom->removeInputSource(audioSource);
                delete this; // whoa whoa whoa holy crap Tim, wow really, are you serious right now? You shouldn't be programming something this bad, could you not have thought of a better solution, really, well, I'm very dissapointed. This is straight crap. Wow. Come on.
            }
        }
    }
    
    
    
private:
    
    /** Interval at which the garbage collection timerCallback is called */
    const int IS_FINISHED_CHECKING_INTERVAL_MILISECONDS = 1000;
    
    /** Positionable audio source to dispose of when done. */
    PositionableAudioSource * audioSource;
    
    /** Mixer to remove the positionable audio source from when done. */
    MixerAudioSource * mixerToRemoveFrom;
};
