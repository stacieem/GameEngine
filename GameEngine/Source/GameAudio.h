//
//  GameAudio.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/19/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SelfDeletingPositionalAudioSource.h"

class GameAudio : public MixerAudioSource
{
public:
    GameAudio();
    ~GameAudio();
    
    
    /** Plays a single audio file. The file can be looped continuously, or
     it can be played once.
     
     @param audioFile    audio file to play
     @param looping      specifies whether or not to continuously loop the
     audio file and play it over and over
     */
    void playAudioFile (File & audioFile, bool looping);
    
private:
    /** Manages the audio formats that can be read */
    AudioFormatManager formatManager;
};
