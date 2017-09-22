/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "CoreEngine.h"


//==============================================================================
CoreEngine::CoreEngine() : Thread("CoreEngine"), gameLogic(gameAudio)
{
    // Setup JUCE Components & Windowing
    addAndMakeVisible (gameView);
    setSize (600, 400);
    
    // Initialize Audio Engine
    setAudioChannels(0, 2); // 0 audio inputs, 2 audio outputs
    
    // Setup InputManager
	inputManager = new InputManager();

    // Create GameModels in memory
    gameModelCurrentFrame = new GameModel();

	renderSwapFrameA = new RenderSwapFrame();
	renderSwapFrameB = new RenderSwapFrame();
    
    // Synchronizatoin - register threads with their correct waitables
    gameLogic.setCoreEngineWaitable (&coreEngineWaitable);
    gameLogic.setLogicWaitable (&logicWaitable);
    
    gameView.setCoreEngineWaitable (&coreEngineWaitable);
    gameView.setRenderWaitable (&renderWaitable);
    // Setup threads to hold pointers to GameModel frames
    gameLogic.setGameModel(gameModelCurrentFrame);
	gameLogic.setRenderSwapFrame(renderSwapFrameA);
	gameView.setRenderSwapFrame(renderSwapFrameB);
    

	// !FIX! MOVE LATER TO AN INPUT MAP AS THE DEFAULT INPUT MAP
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

    // !FIX! Do this in constructor of GameLogic to register InputManger
	gameLogic.registerInputManager(inputManager);

    // Start CoreEngine
    this->startThread();
}

CoreEngine::~CoreEngine()
{
    // Close Audio Engine
    shutdownAudio();
    
    coreEngineWaitable.signal();
    coreEngineWaitable.signal();
    logicWaitable.signal();
    renderWaitable.signal();
    
    gameView.setOpenGLEnabled (false);
    gameLogic.stopThread(500);
    this->stopThread(500);
    
    delete gameModelCurrentFrame;
	delete renderSwapFrameA;
	delete renderSwapFrameB;
}

// JUCE GUI Callbacks ==========================================================

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

// JUCE Audio Callbacks ========================================================

/** Initializes audio engine. Called automatically before the engine begins
    running its audio engine.
 
    Put any code here that creates objects that are dependent on the audio
    engine.
 */
void CoreEngine::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    gameAudio.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

/** Cleans up any leftover data from the audio engine. Called automaticall as
    audio engine is closing.
    
    Put any code here that needs to clean things up once audio is closing down.
 */
void CoreEngine::releaseResources()
{
    gameAudio.releaseResources();
}

/** The main audio callback that is called everytime the audio thread needs
    another block of audio to send to the speakers.
 */
void CoreEngine::getNextAudioBlock (const AudioSourceChannelInfo &bufferToFill)
{
    gameAudio.getNextAudioBlock (bufferToFill);
}

// Engine Thread Callback & Functions ==========================================

/** Swaps the GameModel swap frames between GameLogic and the GameView renderer,
    so that the renderer can render the frame the GameLogic just wrote
 
    !FIX COMMENT!
 */
void CoreEngine::swapRenderFramesBetweenLogicAndRender()
{
	RenderSwapFrame * tempLogicSwapFrame = gameLogic.getRenderSwapFrame();
	gameLogic.setRenderSwapFrame(gameView.getRenderSwapFrame());
	gameView.setRenderSwapFrame(tempLogicSwapFrame);
}

/** Signals both the GameLogic and GameView renderer to start, then swaps their
    GameModel swap frames and repeats.
 */
void CoreEngine::run() {
    while (!threadShouldExit())
    {
		//comment this out to debug properly
		//if (!gameView.hasKeyboardFocus(false)) {
		//	gameView.grabKeyboardFocus();
		//}
        // Allow GameLogic and GameView's rendering to start
        logicWaitable.signal();
        renderWaitable.signal();
        
        // Stop CoreEngine until both GameLogic and GameView's render have finished processing
        coreEngineWaitable.wait();
        coreEngineWaitable.wait();
        
        // Swap render frames
		swapRenderFramesBetweenLogicAndRender();
    }
}
