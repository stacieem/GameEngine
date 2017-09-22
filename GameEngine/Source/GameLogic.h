
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
		inputManager = new InputManager();
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
		inputManager = inputMan;
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
			inputManager->getCommands1(newCommands1);
			for (auto & command : newCommands1)
			{
				switch (command)
				{
				case GameCommand::moveUp:
					gameModelCurrentFrame->getPlayer1()->moveUp();
					break;
				case GameCommand::moveDown:
					gameModelCurrentFrame->getPlayer1()->moveDown();
					break;
				case GameCommand::moveLeft:
					gameModelCurrentFrame->getPlayer1()->moveLeft();
					break;
				case GameCommand::moveRight:
					gameModelCurrentFrame->getPlayer1()->moveRight();
					break;
				case GameCommand::reset:
					gameModelCurrentFrame->getPlayer1()->reset();
					break;
				}
			}

			//locks in the commands for this iteration
			inputManager->getCommands2(newCommands2);
			for (auto & command : newCommands2)
			{
				switch (command)
				{
				case GameCommand::moveUp:
					gameModelCurrentFrame->getPlayer2()->moveUp();
					break;
				case GameCommand::moveDown:
					gameModelCurrentFrame->getPlayer2()->moveDown();
					break;
				case GameCommand::moveLeft:
					gameModelCurrentFrame->getPlayer2()->moveLeft();
					break;
				case GameCommand::moveRight:
					gameModelCurrentFrame->getPlayer2()->moveRight();
					break;
				case GameCommand::reset:
					gameModelCurrentFrame->getPlayer2()->reset();
					break;
				}
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
            gameModelCurrentFrame->processWorldPhysics(deltaTime);

			//copy from currentFrame to swapFrame
			gameModelSwapFrame = gameModelCurrentFrame;

            // Update the GameModel
            // Maybe actions are triggered here ???
            // IMPLEMENT . . .

			// Notify CoreEngine logic is done
			coreEngineWaitable->signal();
		}
	}

	GameModel* gameModelCurrentFrame;
	GameModel* gameModelSwapFrame;
	WaitableEvent* logicWaitable;
	WaitableEvent* coreEngineWaitable;
	//input handling
	InputManager* inputManager;
	Array<GameCommand> newCommands1;
	Array<GameCommand> newCommands2;

	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;

	//Physics World
	WorldPhysics world;
};
