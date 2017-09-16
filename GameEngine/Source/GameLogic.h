
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/** Processes the logic of the game. Started by the Core Engine and manipulates
    the GameDataModel to be rendered for the next frame.
 */
class GameLogic : public Thread
{
public:
	GameLogic() : Thread("GameLogic") {}

	~GameLogic()
    {
        gameModelCurrentFrame = nullptr;
        gameModelSwapFrame = nullptr;
	}

	void setGameModels (GameModel * curentFrame, GameModel * swapFrame)
    {
		gameModelCurrentFrame = curentFrame;
		gameModelSwapFrame = swapFrame;
	}
    
    void setGameModelSwapFrame (GameModel * swapFrame)
    {
        gameModelSwapFrame = swapFrame;
    }
    
    GameModel * getGameModelSwapFrame()
    {
        return gameModelSwapFrame;
    }

	void setCoreEngineWaitable (WaitableEvent * waitable)
    {
		coreEngineWaitable = waitable;
	}

	void setLogicWaitable (WaitableEvent * waitable)
    {
		logicWaitable = waitable;

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
            gameModelCurrentFrame->processWorldPhysics();   // Eventually we want to step by a given time here
            gameModelSwapFrame->processWorldPhysics();      // Eventually we want to step by a given time here
            
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
	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;
};
