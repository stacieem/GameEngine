
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
	GameLogic(GameAudio & gameAudio, CriticalSection * objectDeletionLock) : Thread("GameLogic"), gameAudio(gameAudio)
    {
        //inputManager = new InputManager();
		gamePaused = true;
        this->objectDeletionLock = objectDeletionLock;

		currentTime = Time::currentTimeMillis();
		checkTime = 0;

		currLevel = nullptr;
    }
    
	~GameLogic()
    {
        gameModelCurrentFrame = nullptr;
		delete victory;
		delete gameOver;
	}

	/*
		Establish appearance of victory and gameover levels
	*/
	void createVictory() {
		victory = new Level("Victory");
		victory->addNewBlock();
		victory->getGameObjects().getLast()->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/victory.png"));
		victory->getGameObjects().getLast()->setScale(12, 12);
	}
	void createGameOver() {
		gameOver = new Level("Game Over");
		gameOver->addNewBlock();
		gameOver->getGameObjects().getLast()->getRenderableObject().animationProperties.setIdleTexture(File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/gameOver.png"));
		gameOver->getGameObjects().getLast()->setScale(12, 12);
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
	void playerRespawn() {
		currLevel->resetLevel();
		currLevel->getPlayer(0)->setScore(currLevel->getPlayer(0)->getScore());
		File * audioFile = gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getAudioFileForAction(PhysicalAction::death);

		// If audio file was not in the map, do nothing
		if (audioFile != nullptr)
		{
			gameAudio.playAudioFile(*audioFile, false);
		}
	}
	void playerDied() {
		gameOver->getPlayer(0)->setScore(currLevel->getPlayer(0)->getScore());
		gameOver->getPlayer(0)->setLives(0);
		File * audioFile = gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getAudioFileForAction(PhysicalAction::death);

		// If audio file was not in the map, do nothing
		if (audioFile != nullptr)
		{
			gameAudio.playAudioFile(*audioFile, false);
		}
		currLevel = gameOver;
	}
	bool boundsCollision() {
		bool dead = false;
		if((currLevel->getPlayer(0)->getPhysicsProperties().GetPosition() - 
			currLevel->getGameObjects()[1]->getPhysicsProperties().GetPosition()).y < 1.5) {
			dead = true;
		}
		return dead;
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
	
	void copyPlayerAttributes(Level* currLevel, Level* destLevel) {
		destLevel->getPlayer(0)->setCurrLives(currLevel->getPlayer(0)->getCurrLives());
		
		destLevel->getPlayer(0)->setScore(currLevel->getPlayer(0)->getCurrScore());
		currLevel->resetLevel();
	}

private:

	void run()
    {
        // Set time the very first time GameLogic runs
		gameModelCurrentFrame->setCurrentLevel(0);

		createVictory();
		createGameOver();
		// Main Logic loop
		while (!threadShouldExit())
        {
			// Wait for CoreEngine to signal() this loop
			logicWaitable->wait();
            
            // Grab current level
			if (!gameModelCurrentFrame->getIsGameOver()) {
				currLevel = gameModelCurrentFrame->getCurrentLevel();
			}


			// Calculate time
			newTime = Time::currentTimeMillis();
			deltaTime = newTime - currentTime;
			currentTime = newTime;
			checkTime += deltaTime;

			if (gamePaused || gameModelCurrentFrame->getIsGameOver()) {
				currentTime = Time::currentTimeMillis();
				
			}
			else
			{
				//	process each object (I'm sure if we looked more into contact listeners or bit masking we could've figured this out
				//	however this is the quickest solution i could think of)
				//	ai motions
				if (boundsCollision()) {
					if (gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getCurrLives() - 1 == 0) {
						playerDied();
						gameModelCurrentFrame->setIsGameOver(true);
					}
					else
					{
						playerRespawn();
						gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->
							setCurrLives(gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getCurrLives() - 1);

					}
				}
					for (GameObject* obj : gameModelCurrentFrame->getCurrentLevel()->getGameObjects()) {
						switch (obj->getObjType()) {
						case Enemy:
							((EnemyObject*)(obj))->decision(*gameModelCurrentFrame->getCurrentLevel()->getPlayer(0), deltaTime / 1000);

							if (((EnemyObject*)(obj))->getIsActive() &&
								((EnemyObject*)(obj))->collision(*gameModelCurrentFrame->getCurrentLevel()->getPlayer(0), gameAudio,currLevel->getEnemyPoints())) {
								if (gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getCurrLives() - 1 == 0) {
									playerDied();
									gameModelCurrentFrame->setIsGameOver(true);
								}
								else
								{
									playerRespawn();
									gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->
										setCurrLives(gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->getCurrLives() - 1);

								}

							}
							break;
						case Collectable:
							if (((CollectableObject*)(obj))->collision(*gameModelCurrentFrame->getCurrentLevel()->getPlayer(0), gameAudio)) {
								gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)->addCurrScore(gameModelCurrentFrame->getCurrentLevel()->getCollectablePoints());
							}
							break;
						case Checkpoint:
							GoalPointObject * chkPoint = (GoalPointObject*)obj;
							if (chkPoint->collision(*gameModelCurrentFrame->getCurrentLevel()->getPlayer(0)))
							{
								if (chkPoint->getToWin()) {
									copyPlayerAttributes(currLevel, victory);
									gameModelCurrentFrame->setIsGameOver(true);
									currLevel = victory;
								}
								else if(chkPoint->getLevelToGoTo()-1 != gameModelCurrentFrame->getCurrentLevelIndex())
								{
									copyPlayerAttributes(currLevel, &gameModelCurrentFrame->getLevel(chkPoint->getLevelToGoTo() - 1));
									gameModelCurrentFrame->setCurrentLevel(chkPoint->getLevelToGoTo() - 1);
									currLevel->getPlayer(0)->getPhysicsProperties().setLinearVelocity(0, 0);
								}
							}
							break;
						}
					}
				}
		
			//locks in the commands for this iteration
			inputManager->getCommands(newCommands);

			for (auto & command : newCommands)
			{
				switch (command)
				{
				
					case GameCommand::Player1MoveUp:
						if (!isPaused()) {
							if (!oldCommands.contains(GameCommand::Player1MoveUp)) {
								currLevel->getPlayer(0)->moveUp();
							}
							
						}

						break;
					case GameCommand::Player1MoveDown:
						if (!isPaused()) {
							currLevel->getPlayer(0)->moveDown();
						}

						break;
					case GameCommand::Player1MoveLeft:
						if (!isPaused()) {
							currLevel->getPlayer(0)->moveLeft();
							if (!currLevel->getPlayer(0)->getRenderableObject().animationProperties.getIsAnimating()) {
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setAnimationStartTime(currentTime);
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setLeftAnimation(true);
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setIsAnimating(true);
							}
						}

						break;
					case GameCommand::Player1MoveRight:
						if (!isPaused()) {
							currLevel->getPlayer(0)->moveRight();

							if (!currLevel->getPlayer(0)->getRenderableObject().animationProperties.getIsAnimating()) {
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setAnimationStartTime(currentTime);
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setLeftAnimation(false);
								currLevel->getPlayer(0)->getRenderableObject().animationProperties.setIsAnimating(true);
							}
						}
							
							
						break;
					//Player 2 commands
					case GameCommand::Player2MoveUp:
						if (!isPaused()) {
							currLevel->getPlayer(1)->moveUp();
						}
						break;
					case GameCommand::Player2MoveDown:
						if (!isPaused()) {
							currLevel->getPlayer(1)->moveDown();
						}
						break;
					case GameCommand::Player2MoveLeft:
						if (!isPaused()) {
							currLevel->getPlayer(1)->moveLeft();
						}
						break;
					case GameCommand::Player2MoveRight:
						if (!isPaused()) {
							currLevel->getPlayer(1)->moveRight();
						}
						break;
				}
			}

            // Determine if player is not moving, if so, it should not be animating
			if ((oldCommands.contains(GameCommand::Player1MoveRight) && !newCommands.contains(GameCommand::Player1MoveRight)) ||
				(oldCommands.contains(GameCommand::Player1MoveLeft) && !newCommands.contains(GameCommand::Player1MoveLeft)) ||
				(newCommands.contains(GameCommand::Player1MoveLeft) && newCommands.contains(GameCommand::Player1MoveRight))) {

				if ((!newCommands.contains(GameCommand::Player1MoveLeft) && !newCommands.contains(GameCommand::Player1MoveRight)) ||
					(newCommands.contains(GameCommand::Player1MoveLeft) && newCommands.contains(GameCommand::Player1MoveRight))) {
                    
					currLevel->getPlayer(0)->getRenderableObject().animationProperties.setIsAnimating(false);

				}
			}

			oldCommands = newCommands;
            
            
            // Update gameplay data ============================================
        
            // Grab the camera for the level
            Camera & levelCamera = currLevel->getCamera();
			
            // Only do these things while the game is playing
			if (!gamePaused) {                
				// Process Physics - processes physics and updates objects positions
				currLevel->processWorldPhysics(deltaTime);

				// Play Audio
				// If any new collisions occur, play the specified collision audio
				for (auto & object : currLevel->getGameObjects())
				{
					if (object->getRenderableObject().animationProperties.getCanimate()) {
						if (object->getRenderableObject().animationProperties.getIsAnimating()) {
							object->getRenderableObject().animationProperties.updateAnimationCurrentTime(currentTime);
						}
					}

					if (object->getPhysicsProperties().hasNewCollisions())
					{
                        File * audioFile = object->getAudioFileForAction(PhysicalAction::collsion);
                     
                        // If audio file was not in the map, do nothing
                        if (audioFile != nullptr)
                        {
                            gameAudio.playAudioFile(*audioFile, false);
                        }
					}
				}
                
                // Update camera position based on the position of player 1
                // The player1 object will be unmoving, while the world moves around it
                //levelCamera.setXPosition(-currLevel->getPlayer(0)->getRenderableObject().position.x);
                levelCamera.setPositionXY(-currLevel->getPlayer(0)->getRenderableObject().position.x, 0.0f);
			}
            
            
            // Update render data ==============================================
            /** Always render, regardless of pause/play */
            
            // Set camera view matrix
            renderSwapFrame->setViewMatrix(levelCamera.getViewMatrix());
            
        
            /** FUTURE EFFICIENCY FEATURE:
                Add in some pre-render visiblity checking. If an object is
                obviously going to be out of view, do not put it in a render
                frame.
             */

            
            // GameObject deletion is a race condition, because a deleted object
            // could have a function called on it such as: getRenderableObject()
            // Therefore, we must lock here
            objectDeletionLock->enter();
            
                // Create array of potentially renderable objects in view
                /** FUTURE EFFICIENCY FEATURE:
                    Add in some pre-render visiblity checking. If an object is
                    obviously going to be out of view, do not put it in a render
                    frame.
                 */

                vector<RenderableObject> renderableObjects;
                for (auto gameObject : currLevel->getGameObjects())
                {
                    if (gameObject->isRenderable())
                    {
                        renderableObjects.push_back(gameObject->getRenderableObject());
                        
                        // If the game is playing, make sure no object is selected
                        if (!isPaused())
                        {
                            renderableObjects.back().isSelected = false;
                        }
                    }
                }
            
            objectDeletionLock->exit();
            
            // Add the renderables to the swap frame to send to GameView
            renderSwapFrame->setRenderableObjects(renderableObjects);
 
			//Add player attributes we want to the render swap frame
			renderSwapFrame->setAttribute("score", currLevel->getPlayer(0)->getCurrScore());
			renderSwapFrame->setAttribute("lives", currLevel->getPlayer(0)->getCurrLives());

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
	Array<GameCommand> oldCommands;


	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	int64 gameLoopTime;

	//Physics World
	WorldPhysics world;

	bool gamePaused;
	int64 checkTime;

	//end game Screens
	Level* victory;
	Level* gameOver;
	Level * currLevel;
    // GameModel Object synchronization
    CriticalSection * objectDeletionLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameLogic)
};
