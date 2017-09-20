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
	inputManager = new InputManager();
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
   
//    logicSwapFrameContainer = &gameModelSwapFrameA;
//    renderSwapFrameContainer = &gameModelSwapFrameB;

    gameLogic.setGameModels (gameModelCurrentFrame, gameModelSwapFrameA);
    gameView.setGameModelSwapFrame (gameModelSwapFrameB);
    

	//add the commands!!!!!
	KeyPress aKey('w');
	inputManager->addCommand(aKey, GameCommand::moveUp);
	aKey = KeyPress('s');
	inputManager->addCommand(aKey, GameCommand::moveDown);
	aKey = KeyPress('a');
	inputManager->addCommand(aKey, GameCommand::moveLeft);
	aKey = KeyPress('d');
	inputManager->addCommand(aKey, GameCommand::moveRight);
	aKey = KeyPress('r');
	inputManager->addCommand(aKey, GameCommand::reset);


	aKey = KeyPress('i');
	inputManager->addCommand2(aKey, GameCommand::moveUp);
	aKey = KeyPress('k');
	inputManager->addCommand2(aKey, GameCommand::moveDown);
	aKey = KeyPress('j');
	inputManager->addCommand2(aKey, GameCommand::moveLeft);
	aKey = KeyPress('l');
	inputManager->addCommand2(aKey, GameCommand::moveRight);
	aKey = KeyPress('r');
	inputManager->addCommand2(aKey, GameCommand::reset);
    // Start the GameLogic thread and the GameView's renderer
    gameLogic.startThread();
    gameView.setOpenGLEnabled (true);
	gameView.setWantsKeyboardFocus(true);
	// setup inputManager Listeners
	getTopLevelComponent()->addKeyListener(inputManager);
	getTopLevelComponent()->addMouseListener(inputManager, true);

	gameLogic.setCommands(inputManager);


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
    
    delete gameModelCurrentFrame;
    delete gameModelSwapFrameA;
    delete gameModelSwapFrameB;
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
    GameModel * tempLogicSwapFrame = gameLogic.getGameModelSwapFrame();
    gameLogic.setGameModelSwapFrame(gameView.getGameModelSwapFrame());
    gameView.setGameModelSwapFrame(tempLogicSwapFrame);
}

/** Signals both the GameLogic and GameView renderer to start, then swaps their
    GameModel swap frames and repeats.
 */
void CoreEngine::run() {
    while (!threadShouldExit())
    {
		//comment this out to debug properly
		if (!gameView.hasKeyboardFocus(false)) {
			gameView.grabKeyboardFocus();
		}
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
