
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
		gamePaused = false;
    }
    
	~GameLogic()
    {
        gameModelCurrentFrame = nullptr;
	}

	/** Sets whether or not the game is paused.
	*/
	void setPaused(bool paused)
	{
		gamePaused = paused;
	}

	/* Gets the current pause/play state of the game*/
	bool isPaused() {
		return gamePaused;
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

		Level * currLevel = gameModelCurrentFrame->getCurrentLevel();
		// Main Logic loop
		while (!threadShouldExit())
        {
			// Wait for CoreEngine to signal() this loop
			logicWaitable->wait();
            
            // Grab current level
            currLevel = gameModelCurrentFrame->getCurrentLevel();

			if (gamePaused) {
				currentTime = Time::currentTimeMillis();
			}

			// Calculate time
			newTime = Time::currentTimeMillis();
			deltaTime = newTime - currentTime;
			currentTime = newTime;
			checkTime += deltaTime;

			//locks in the commands for this iteration
			inputManager->getCommands(newCommands);

			for (auto & command : newCommands)
			{
				switch (command)
				{
					//Apparently if statements inside a switch are a thing, probably a bad thing
					//Don't accept this input if the game is paused
					if (!isPaused()) {

						case GameCommand::Player1MoveUp:
							DBG("MOVE UP");
							currLevel->getPlayer(0)->moveUp();
							break;
						case GameCommand::Player1MoveDown:
							currLevel->getPlayer(0)->moveDown();
							break;
						case GameCommand::Player1MoveLeft:
							currLevel->getPlayer(0)->moveLeft();
							break;
						case GameCommand::Player1MoveRight:
							currLevel->getPlayer(0)->moveRight();
							break;
						//Player 2 commands
						case GameCommand::Player2MoveUp:
							DBG("2MOVE UP");
							currLevel->getPlayer(1)->moveUp();
							break;
						case GameCommand::Player2MoveDown:
							currLevel->getPlayer(1)->moveDown();
							break;
						case GameCommand::Player2MoveLeft:
							currLevel->getPlayer(1)->moveLeft();
							break;
						case GameCommand::Player2MoveRight:
							currLevel->getPlayer(1)->moveRight();
							break;
						case GameCommand::reset:
							currLevel->getPlayer(0)->reset();
							currLevel->getPlayer(1)->reset();
							break;
					}
				}
				
				
			}
			
            //Only do these things if the game is not paused
			if (!gamePaused) {
				// Process Physics
				currLevel->processWorldPhysics(deltaTime);

				// Play Audio
				// If any new collisions occur, play the specified collision audio
				for (auto & object : currLevel->getGameObjects())
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
			}
            
            
            // Update the GameModel
			//Update the number of DrawableObjects in the RenderSwapFrame
			renderSwapFrame->setDrawableObjectsLength(currLevel->getNumGameObjects());
            // Yo, if the number of drawable objects is the same as the number of
            // objects that are in the level, then there is no point in having two
            // types of objects. We just want ONE type of GameObject that has an
            // ability to be drawable. Maybe it has a method: isDrawable() that
            // returns whether or not the object is drawable. If it is drawable,
            // load its textures where applicable, if not, load a default color
            // or something. From what I can tell, we do not need two types
            // of game objects.

			for (int i = 0; i < currLevel->getGameObjects().size(); i++)
			{
				renderSwapFrame->setDrawableObjectVertices(currLevel->getGameObjects()[i]->getVertices(), i);
				renderSwapFrame->setDrawableObjectTexture(currLevel->getGameObjects()[i]->getTexture(), i);
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
	Array<GameCommand> newCommands;

	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;

	//Physics World
	WorldPhysics world;

	bool gamePaused;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameLogic)
};
