
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameAudio.h"
#include "PhysicalAction.h"

/** Processes the logic of the game. Started by the Core Engine and manipulates
    the GameDataModel to be rendered for the next frame.
 */
class GameLogic : public Thread
{
public:
	GameLogic(GameAudio & gameAudio) : Thread("GameLogic"), gameAudio(gameAudio)
    {
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
            //gameModelSwapFrame->processWorldPhysics();
            
            // SUPER BAD HACK RIGHT NOW DONT EVER DO THIS
            gameModelSwapFrame = gameModelCurrentFrame;
            
            // If any new collisions occur, play the specified collision audio
            for (auto & object : gameModelCurrentFrame->getGameObjects())
            {
                if (object->getPhysicsProperties().hasNewCollisions())
                {
                    //File * audioFile = object->getAudioFileForAction(PhysicalAction::collsion);
                 
                    // If audio file was not in the map, do nothing
//                    if (audioFile != nullptr)
//                    {
//                        gameAudio.playAudioFile(*audioFile, false);
//                    }
                    gameAudio.playAudioFile(object->getAudioFile(), false);
                    
                }
            }
            
            // Update the GameModel
            // Maybe actions are triggered here ???
            // IMPLEMENT . . .

			// Notify CoreEngine logic is done
			coreEngineWaitable->signal();
		}
	}

    GameAudio & gameAudio;
	GameModel* gameModelCurrentFrame;
	GameModel* gameModelSwapFrame;
	WaitableEvent* logicWaitable;
	WaitableEvent* coreEngineWaitable;
	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;
};
