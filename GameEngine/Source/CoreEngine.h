

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameView.h"
#include "GameModel.h"
#include "GameLogic.h"


/** Represents the core of the entire game engine, including the game's data
    models: GameModels, the game's rendered view: GameView, and the game's
    logic content: GameLogic. CoreEngine runs each of these components and
    coordinates their interaction with one another.
 
    This component lives inside our window, and contains controls and content of
    the GameView.
*/
class CoreEngine    : public Component, public Thread
{
public:
    //==========================================================================
    CoreEngine();
    ~CoreEngine();

    void paint (Graphics&) override;
    void resized() override;
    
    // Thread running callback =================================================
    void run() override;
    
    
    /** Swaps the GameModel swap frames between GameLogic and the GameView renderer,
        so that the renderer can render the frame the GameLogic just wrote
     */
    void swapFramesBetweenLogicAndRender();

private:
    //==========================================================================
    GameView gameView;
    GameLogic gameLogic;
    
    // Game Models - each model represents a frame of state of the game.
    //
    //      GameLogic is always in charge of processing the gameModelCurrentFrame
    //      because logic must be processed on a scene before it can be rendered.
    //      GameLogic also copies this processed data model into the current
    //      gameModelSwapFrame that it holds.
    //
    //      GameView's renderer is in charge of rendering the gameModelSwapFrame
    //      that it currently holds.
    //
    //      Once GameLogic and GameView's renderer have gone through their first
    //      iteration, CoreEngine will swap the gameModelSwapFrames that GameLogic
    //      and GameView hold. At this point GameLogic and GameView's renderer
    //      can start their next iteration, repeating the above process.
    //
    //      So GameLogic always processes the current frame (overrwriting its
    //      gameModelSwapFrame) while GameRenderer renders the last frame, and
    //      then they swap frames and repeat.
    //
    GameModel * gameModelCurrentFrame;
    GameModel * gameModelSwapFrameA;
    GameModel * gameModelSwapFrameB;

    // Thread Synchronization between CoreEngine, GameView, and GameLogic
    WaitableEvent logicWaitable;
    WaitableEvent renderWaitable;
    WaitableEvent coreEngineWaitable;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreEngine)
};
