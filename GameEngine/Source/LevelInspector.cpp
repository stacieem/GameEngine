#include "LevelInspector.h" 


LevelInspector::LevelInspector(WorldNavigator & worldNavigator) :
        SELECTED_ROW_COLOUR(0xFF4091F5),
        worldNavigator(worldNavigator) {
    
    addAndMakeVisible(propertyPanel);
    addAndMakeVisible(playButton);
    playButton.setButtonText("Start Game");
    playButton.addListener(this);
    selectedObjectValue.addListener(this);

	saveLevelButton.setButtonText("Save Game");
	saveLevelButton.addListener(this);


    addAndMakeVisible(levelLabel);
    addAndMakeVisible(levelComboBox);
    addAndMakeVisible(addLevelButton);
    addAndMakeVisible(removeLevelButton);
	addAndMakeVisible(resetLevelButton);
	addAndMakeVisible(resetGameButton);
	addAndMakeVisible(saveLevelButton);

    addLevelButton.setButtonText("+");
    removeLevelButton.setButtonText("-");
    levelLabel.setJustificationType(Justification::centred);

	resetLevelButton.setButtonText("Restart Level");
	resetGameButton.setButtonText("Restart Game");
    addLevelButton.addListener(this);
    removeLevelButton.addListener(this);
	resetLevelButton.addListener(this);
	resetGameButton.addListener(this);
    levelComboBox.addListener(this);
	hasTimer.addListener(this);
	hasScore.addListener(this);
    gravity.addListener(this);
	hasCheckPoint.addListener(this);
}

LevelInspector::~LevelInspector() {}

void LevelInspector::setCoreEngine(CoreEngine* engine)
{
    coreEngine = engine;
}

// JUCE GUI Callbacks ======================================================
void LevelInspector::paint(Graphics& g)
{
    //g.fillAll(Colours::indigo);
}

void LevelInspector::updateInspector(GameModel & gameModel)
{
	// Update Selected Level Index
	selectedLevelIndex = gameModel.getCurrentLevelIndex();

	// Update Selected Level and Select first Game Object
	if (gameModel.getCurrentLevel() != selectedLevel) {
		selectedLevel = gameModel.getCurrentLevel();
	}

	// Clear data to reload with new data
	propertyPanel.clear();
	levelObjGraphProperties.clear();
	levelPhysicsProperties.clear();
	levelAudioProperties.clear();
	levelBackgroundProperties.clear();
	levelObjConditionalProperties.clear();
	levelComboBox.clear(NotificationType::dontSendNotification);


	// Add all the Levels to the Level Inspector's comboBox selector
	for (int i = 0; i < gameModel.getNumLevels(); ++i)
	{
		levelComboBox.addItem(String(i + 1), i + 1);

		// If the current index is the level, make it the selected level
		if (i == selectedLevelIndex)
		{
			levelComboBox.setSelectedItemIndex(i, NotificationType::dontSendNotification);
		}
	}


	// Set the current level text
	levelLabel.setText("Level: ", NotificationType::dontSendNotification);

    // Use selected object array to color rows below
    Array<GameObject *> selectedObjects = worldNavigator.getSelectedObjects();

	// Populate object graph with GameObjects of currently selected level
	for (GameObject * gameObj : selectedLevel->getGameObjects())
	{
		// Add to internal array of game objects???
		if (gameObj->getName() != "Killing Floor") {
			gameObjects.addIfNotAlreadyThere(gameObj);
			SelectObjectButtonPropertyComponent* objectRow =
				new SelectObjectButtonPropertyComponent(gameObjects.indexOf(gameObj),
					selectedObjectValue,
					gameObj->getName(),
					false);

			// If row of a selected object, set its colour to be highlighted
			if (selectedObjects.contains(gameObj))
			{
				objectRow->setColour(PropertyComponent::ColourIds::backgroundColourId,
					SELECTED_ROW_COLOUR);
			}

			// Add the row to the graph
			levelObjGraphProperties.add(objectRow);
		}
	}

	propertyPanel.addSection("Object Graph", levelObjGraphProperties);


	//add Level Physics
	gravity.setValue(var((int)1));
	ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(gravity, "Gravity:");
	combo->setTextWhenNothingSelected("Choose Gravity");
	combo->addItem("High gravity", 3);
	combo->addItem("Anti Gravity", 2);
	combo->addItem("Normal", 1);
	combo->setSelectedId(selectedLevel->getGravityState() + 1, dontSendNotification);
	levelPhysicsProperties.add(combo);

	propertyPanel.addSection("World Physics", levelPhysicsProperties);

	//add Level Audio



	//add Level Background


    propertyPanel.refreshAll();
}


void LevelInspector::textPropertyComponentChanged(TextPropertyComponent * component)
{
    if (component->getName() == "Object Name:")
    {
        updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
    }
    // NOTE: If we ever add another textPropertyComponent condition here, we MUST
    // use else if. Since we are sending "Synchronous" change messages, this
    // inspector will be updated too. This means that after that call, any
    // modifications to the TextPropertyComponent pointer of this function could
    // be bad because the TextPropertyComponent pointer could have been deallocated
}
void LevelInspector::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    int lineHeight = 50;

    // Play/Pause Button
    playButton.setBounds(bounds.removeFromTop(lineHeight));
	saveLevelButton.setBounds(bounds.removeFromTop(lineHeight));
	resetLevelButton.setBounds(bounds.removeFromTop(lineHeight));
	resetGameButton.setBounds(bounds.removeFromTop(lineHeight));
    // Level Selection
    juce::Rectangle<int> levelSelectRow = bounds.removeFromTop(lineHeight);


    Font levelLabelFont;
    levelLabelFont.setHeight(levelSelectRow.getHeight() - 20);
    levelLabel.setFont(levelLabelFont);
    levelLabel.setBounds(levelSelectRow.removeFromLeft(levelSelectRow.getWidth() / 2));
    levelComboBox.setBounds(levelSelectRow.removeFromLeft(levelSelectRow.getWidth() * 2 / 3));
    addLevelButton.setBounds(levelSelectRow.removeFromTop(levelSelectRow.getHeight() / 2));
    removeLevelButton.setBounds(levelSelectRow);

    // Property Panels
    propertyPanel.setBounds(bounds);
}

void LevelInspector::buttonClicked(Button * button) {

    if (button == &playButton)
    {
        if (button->getToggleState())
        {
            button->setButtonText("Start Game");
            button->setToggleState(false, NotificationType::dontSendNotification);
        }
        else
        {
            button->setButtonText("Stop Game");
            button->setToggleState(true, NotificationType::dontSendNotification);

        }
        
        coreEngine->toggleGamePause();
        updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
    }
    else if (button == &addLevelButton)
    {
        // Add level and update inspectors
        coreEngine->addLevel();
        updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
    }
    else if (button == &removeLevelButton)
    {
        coreEngine->removeLevel(selectedLevelIndex);
        
        // NOTE: THIS IS VERY BAD
        // This should be synchronous. I had to make it asynchronous because I
        // was getting an error and didn't have time for a better solution.
        // This is only masking a deeper error:
        // When we delete a level, there is data being referenced in the level
        // (Model data) that is still used in Logic and/or render. We would
        // have to wait for two loops of core engine to finish to properly
        // delete it, but we are lucky in that, this asynchronous message
        // causes enough delay that there is no bad access error.
        updateInspectorsChangeBroadcaster->sendChangeMessage();
        
        
    } else if (button == &saveLevelButton) {

        coreEngine->saveGame();
		AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
			"Game Saved",
			"");

    }
    else if (button == &resetLevelButton)
    {
        selectedLevel->resetLevel();
    }
	else if (button == &resetGameButton)
	{
		for (auto level : coreEngine->getGameModel().getLevels()) {
			level->resetLevel();
			level->getPlayer(0)->setScore(0);
			level->getPlayer(0)->setCurrLives(level->getPlayer(0)->getLives());
		}
		coreEngine->getGameModel().setCurrentLevel(0);
		coreEngine->getGameModel().setIsGameOver(false);
		updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
	}
}

void LevelInspector::setChildrenEnabled(bool shouldBeEnabled)
{
    propertyPanel.setEnabled(shouldBeEnabled);
    levelComboBox.setEnabled(shouldBeEnabled);
    addLevelButton.setEnabled(shouldBeEnabled);
    removeLevelButton.setEnabled(shouldBeEnabled);
    levelLabel.setEnabled(shouldBeEnabled);
    resetLevelButton.setEnabled(shouldBeEnabled);
	resetGameButton.setEnabled(shouldBeEnabled);
    saveLevelButton.setEnabled(shouldBeEnabled);
}

void LevelInspector::valueChanged(Value &value)
{
    if (value.refersToSameSourceAs(gravity)) {

        switch ((int)gravity.getValue()) {
        case 1:
            selectedLevel->getWorldPhysics().setGravity(WorldPhysics::Normal);
            break;
        case 2:
            selectedLevel->getWorldPhysics().setGravity(WorldPhysics::AntiGrav);
            break;
        case 3:
            selectedLevel->getWorldPhysics().setGravity(WorldPhysics::HighGrav);
            break;
        }
    }
    // Object Selection Button
    else if (value.refersToSameSourceAs(selectedObjectValue))
    {
        // Update navigator of selected object
        worldNavigator.setSelectedObject (gameObjects[(int)value.getValue()]);
    }
}



void LevelInspector::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &levelComboBox)
    {
        // Select Level
        // Update Inspector
        coreEngine->setCurrentLevel(comboBoxThatHasChanged->getSelectedItemIndex());

		updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
    }
	
}
