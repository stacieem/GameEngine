/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "CoreEngine.h"


//==============================================================================
CoreEngine::CoreEngine() : Thread("CoreEngine")
{
    // Setup JUCE Components & Windowing
    addAndMakeVisible (gameView);
    setSize (600, 400);
    
    // Create GameModels in memory
    gameModelCurrentFrame = new GameModel();
    gameModelSwapFrameA = new GameModel();
    gameModelSwapFrameB = new GameModel();
    
    // Synchronizatoin - register threads with their correct waitables
    gameLogic.setCoreEngineWaitable (&coreEngineWaitable);
    gameLogic.setLogicWaitable (&logicWaitable);
    
    gameView.setCoreEngineWaitable (&coreEngineWaitable);
    gameView.setRenderWaitable (&renderWaitable);
    
    // Setup threads to hold pointers to GameModel frames
    gameLogic.setGameModels (gameModelCurrentFrame, gameModelSwapFrameA);
    gameView.setGameModelSwapFrame (gameModelSwapFrameB);
    
    // Start the GameLogic thread and the GameView's renderer
    gameLogic.startThread();
    gameView.setOpenGLEnabled (true);
    
    this->startThread();
}

CoreEngine::~CoreEngine()
{
    coreEngineWaitable.signal();
    coreEngineWaitable.signal();
    logicWaitable.signal();
    renderWaitable.signal();
    
    gameView.setOpenGLEnabled (false);
    gameLogic.stopThread(500);
    this->stopThread(500);
}

void CoreEngine::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void CoreEngine::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    gameView.setBounds (getLocalBounds());
}

/** Swaps the GameModel swap frames between GameLogic and the GameView renderer,
    so that the renderer can render the frame the GameLogic just wrote
 */
void CoreEngine::swapFramesBetweenLogicAndRender()
{
    GameModel * tempLogicGameModel = gameLogic.getGameModelSwapFrame();
    gameLogic.setGameModelSwapFrame (gameView.getGameModelSwapFrame());
    gameView.setGameModelSwapFrame (tempLogicGameModel);
}

/** Signals both the GameLogic and GameView renderer to start, then swaps their
    GameModel swap frames and repeats.
 */
void CoreEngine::run() {
    
    while (!threadShouldExit())
    {
        // Allow GameLogic and GameView's rendering to start
        logicWaitable.signal();
        renderWaitable.signal();
        
        // Stop CoreEngine until both GameLogic and GameView's render have finished processing
        coreEngineWaitable.wait();
        coreEngineWaitable.wait();
        
        // Swap Pointers
        swapFramesBetweenLogicAndRender();
    }
}
