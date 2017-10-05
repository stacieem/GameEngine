
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameAudio.h"
#include "PhysicalAction.h"

#include "InputManager.h"
#include "RenderSwapFrame.h"


/** Processes the logic of the game. Started by the Core Engine and manipulates
    the GameDataModel to be rendered for the next frame.
 */
class GameLogic : public Thread
{
public:
	GameLogic(GameAudio & gameAudio) : Thread("GameLogic"), gameAudio(gameAudio)
    {
        //inputManager = new InputManager();
    }
    
	~GameLogic()
    {
        gameModelCurrentFrame = nullptr;
	}

    /** Sets the GameModel current frame being processed for logic, and the
        GameModel swap frame that will be swapped with the GameView to be rendered.
     */
	void setGameModel(GameModel * curentFrame)
    {
		gameModelCurrentFrame = curentFrame;
	}
    

	/** Sets the Render swap frame that will be processed for logic before it
	is sent to the GameView to be rendered.
	*/
	void setRenderSwapFrame(RenderSwapFrame * swapFrame)
	{
		renderSwapFrame = swapFrame;
	}

	/** Returns the GameModel swap frame that the GameLogic is currently
	processing.
	*/
	RenderSwapFrame * getRenderSwapFrame()
	{
		return renderSwapFrame;
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
	void registerInputManager(InputManager* inputManager) {
		this->inputManager = inputManager;
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
            
            // Process Physics
            gameModelCurrentFrame->processWorldPhysics(deltaTime);
            
            // Play Audio
            // If any new collisions occur, play the specified collision audio
            for (auto & object : gameModelCurrentFrame->getGameObjects())
            {
                if (object->getPhysicsProperties().hasNewCollisions())
                {
//                    File * audioFile = object->getAudioFileForAction(PhysicalAction::collsion);
//                 
//                     If audio file was not in the map, do nothing
//                    if (audioFile != nullptr)
//                    {
//                        gameAudio.playAudioFile(*audioFile, false);
//                    }
                    gameAudio.playAudioFile(object->getAudioFile(), false);
                    
                }
            }
            
            // Update the GameModel
			//Update the number of DrawableObjects in the RenderSwapFrame
			renderSwapFrame->setDrawableObjectsLength(gameModelCurrentFrame->getNumGameObjects());

			for (int i = 0; i < gameModelCurrentFrame->getGameObjects().size(); i++)
			{
				renderSwapFrame->setDrawableObjectVertices(gameModelCurrentFrame->getGameObjects()[i]->getVertices(), i);
				renderSwapFrame->setDrawableObjectTexture(gameModelCurrentFrame->getGameObjects()[i]->getTexture(), i);

			}

			if (threadShouldExit()) {
				break;
			}
            
            // Maybe actions are triggered here ???
            // IMPLEMENT . . .

			// Notify CoreEngine logic is done
			coreEngineWaitable->signal();
		}
	}

    GameAudio & gameAudio;
	GameModel* gameModelCurrentFrame;
	RenderSwapFrame* renderSwapFrame;
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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameLogic)
};
