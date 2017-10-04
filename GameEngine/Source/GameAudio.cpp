//
//  GameAudio.cpp
//  GameEngine
//
//  Created by Tim Arterbury on 9/19/17.
//
//

#include "GameAudio.h"


GameAudio::GameAudio()
{
    formatManager.registerBasicFormats();
}

GameAudio::~GameAudio()
{
    // MixerAudioSource shutdown
    this->removeAllInputs();    // Unregister and delete all AudioFormatReaderSources currently connected
    this->releaseResources();
}


void GameAudio::playAudioFile (File & audioFile, bool looping)
{
    AudioFormatReader* reader = formatManager.createReaderFor (audioFile);
    
    if (reader != nullptr)
    {
        AudioFormatReaderSource * newSource = new AudioFormatReaderSource (reader, true);
                                                            // 2nd parameter, "true", in this constructor
                                                            // specifies to delete the AudioFormatReader
                                                            // when the AudioFormatReaderSouce is deleted
        newSource->setLooping(looping);
            
        this->addInputSource (newSource, true);
                                        // 2nd parameter, "false", specifies to not
                                        // delete the AudioFormatReaderSource when
                                        // removed from the MixerAudioSource, since
                                        // we are storing them in an OwnedArray that
                                        // will later deallocate them.
        
        // FIX MEMORY LEAK: If it is looping, create a new kind of
        // audio source that gets notified when to quit and dispose of itself.
        // Right now, when a looping audio source is used inside a
        // SelfDeletingPositionalAudioSource, the audio will loop forever and
        // never disconnect itself from the Mixer. We need to deal with this.
        
        new SelfDeletingPositionalAudioSource(newSource, this); // Whoa whoa whoa this is crazy dude, calm the crap down. Who do you think you are?
    }
}
