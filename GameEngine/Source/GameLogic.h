
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "InputManager.h"

/** Processes the logic of the game. Started by the Core Engine and manipulates
    the GameDataModel to be rendered for the next frame.
 */
class GameLogic : public Thread
{
public:
	GameLogic() : Thread("GameLogic"), world() { 
		commands = new InputManager();
		oldCommands = commands->getCommands();
	}

	~GameLogic()
    {
        gameModelCurrentFrame = nullptr;
        gameModelSwapFrame = nullptr;
	}

    /** Sets the GameModel current frame being processed for logic, and the
        GameModel swap frame that will be swapped with the GameView to be rendered.
     */
	void setGameModels (GameModel * curentFrame, GameModel * swapFrame)
    {
		gameModelCurrentFrame = curentFrame;
		gameModelSwapFrame = swapFrame;
	}
    
    /** Sets the GameModel swap frame that will be processed for logic before it
        is sent to the GameView to be rendered.
     */
    void setGameModelSwapFrame (GameModel * swapFrame)
    {
        gameModelSwapFrame = swapFrame;
    }
    
    /** Returns the GameModel swap frame that the GameLogic is currently
        processing.
     */
    GameModel * getGameModelSwapFrame()
    {
        return gameModelSwapFrame;
    }

    /** Sets the WaitableEvent for the GameLogic to signal the CoreEngine when
        it is done processing.
     */
	void setCoreEngineWaitable (WaitableEvent * waitable)
    {
		coreEngineWaitable = waitable;
	}

    /** Sets the WaitableEvent for the GameLogic to wait until the CoreEngine
        has signaled it to go after the swap frames have been swapped.
     */
	void setLogicWaitable (WaitableEvent * waitable)
    {
		logicWaitable = waitable;

	}

	/* Sets the InputManager to match the values of the CoreEngine 
		InputManager
	*/
	void setCommands(InputManager* inputMan) {
		commands = inputMan;
	}

private:


	void run()
    {
        // Set time the very first time GameLogic runs
		currentTime = Time::currentTimeMillis();
		int64 checkTime = 0;

		// Main Logic loop
		while (!threadShouldExit())
        {
			// Wait for CoreEngine to signal() this loop
			logicWaitable->wait();

			//locks in the commands for this iteration
			newCommands = commands->getCommands();

			if (newCommands[0] ) {
				gameModelCurrentFrame->getPlayer()->moveUp();
			}
			if (newCommands[1] ) {
				gameModelCurrentFrame->getPlayer()->moveDown();
			}
			if (newCommands[2]) {
				gameModelCurrentFrame->getPlayer()->moveLeft();
			}
			if (newCommands[3] ) {
				gameModelCurrentFrame->getPlayer()->moveRight();
			}
			if (newCommands[4]) {
				gameModelCurrentFrame->getPlayer()->reset();
			}

            // Calculate time
			newTime = Time::currentTimeMillis();
			deltaTime = newTime - currentTime;
			currentTime = newTime;
			checkTime += deltaTime;

            // TESTTTTT
			// For every second, update the calculated frame rate
			if (checkTime > 1000) {
				checkTime = 0;
				DBG(1000.0/deltaTime);
                // FIX: Update the frame rate in the GameModel
			}
            
            // Process Physics
            //gameModelCurrentFrame->processWorldPhysics();   // Eventually we want to step by a given time here
            gameModelCurrentFrame->processWorldPhysics();

			//copy from currentFrame to swapFrame
			gameModelSwapFrame = gameModelCurrentFrame;

            // Update the GameModel
            // Maybe actions are triggered here ???
            // IMPLEMENT . . .
			oldCommands = newCommands;
			commands->reset();
			//commands->reset();
			// Notify CoreEngine logic is done
			coreEngineWaitable->signal();
		}
	}

	GameModel* gameModelCurrentFrame;
	GameModel* gameModelSwapFrame;
	WaitableEvent* logicWaitable;
	WaitableEvent* coreEngineWaitable;
	//input handling
	InputManager* commands;
	std::vector<bool> oldCommands;
	std::vector<bool> newCommands;

	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;

	//Physics World
	WorldPhysics world;
};
