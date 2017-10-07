//
//  GameAudio.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/19/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class GameAudio : public MixerAudioSource, public Timer
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
    void playAudioFile (File audioFile, bool looping);
    
    
    /** AudioSource Garbage Collection
     
        Repeatedly checks if any of the current audioSources are done playing,
        if so, it deletes them.
     */
    void timerCallback();
    
private:
    
    /** Interval at which the garbage collection timerCallback is called */
    const int GARBAGE_COLLECTION_INTERVAL_MILISECONDS = 1000;
    
    /** Manages the audio formats that can be read */
    AudioFormatManager formatManager;
    
    /** Audio sources created and triggered by various logic and actions in the game */
    //OwnedArray<AudioFormatReaderSource, CriticalSection> audioSources;
    
    /** Lock for when MixerAudioSource and audioSources so they are attomically
        updated together
     */
    //CriticalSection lock;

	JUCE_LEAK_DETECTOR(GameAudio)
};
