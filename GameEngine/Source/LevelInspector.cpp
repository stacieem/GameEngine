#include "LevelInspector.h" 

LevelInspector::LevelInspector(WorldNavigator & worldNavigator) :
        SELECTED_ROW_COLOUR(0xFF4091F5),
        worldNavigator(worldNavigator) {
    
    addAndMakeVisible(propertyPanel);
    addAndMakeVisible(playButton);
    playButton.setButtonText("Start Game");
    playButton.addListener(this);
    selectedObjectValue.addListener(this);


    addAndMakeVisible(levelLabel);
    addAndMakeVisible(levelComboBox);
    addAndMakeVisible(addLevelButton);
    addAndMakeVisible(removeLevelButton);
	addAndMakeVisible(resetLevelButton);

    addLevelButton.setButtonText("+");
    removeLevelButton.setButtonText("-");
    levelLabel.setJustificationType(Justification::centred);

	resetLevelButton.setButtonText("Reset Current Level");
    addLevelButton.addListener(this);
    removeLevelButton.addListener(this);
	resetLevelButton.addListener(this);
    levelComboBox.addListener(this);
	hasTimer.addListener(this);
	hasScore.addListener(this);
    gravity.addListener(this);
	hasCheckPoint.addListener(this);
}
LevelInspector::~LevelInspector() {

}

void LevelInspector::setCoreEngine(CoreEngine* engine) {
    coreEngine = engine;
}
// JUCE GUI Callbacks ======================================================
void LevelInspector::paint(Graphics& g) {
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


    // Populate object graph with GameObjects of currently selected level
    for (auto gameObj : selectedLevel->getGameObjects())
    {
        // Add to internal array of game objects???
        gameObjects.addIfNotAlreadyThere(gameObj);
        SelectObjectButtonPropertyComponent* objectRow =
            new SelectObjectButtonPropertyComponent(gameObjects.indexOf(gameObj),
                                                    selectedObjectValue,
                                                    gameObj->getName(),
                                                    false);
       
        // If row of selected object, set its colour to be highlighted
        if (gameObj == worldNavigator.getSelectedObject())
        {
            objectRow->setColour(PropertyComponent::ColourIds::backgroundColourId,
                                 SELECTED_ROW_COLOUR);
        }
        
        // Add the row to the graph
        levelObjGraphProperties.add(objectRow);
    }
    
    propertyPanel.addSection("Object Graph", levelObjGraphProperties);


    //add Level Physics
    gravity.setValue(var((int)1));
    ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(gravity, "Gravity:");
    combo->setTextWhenNothingSelected("Choose Gravity");
    combo->addItem("High gravity", 3);
    combo->addItem("Anti Gravity", 2);
    combo->addItem("Normal", 1);
	combo->setSelectedId(selectedLevel->getGravityState()+1,dontSendNotification);
    levelPhysicsProperties.add(combo);

    propertyPanel.addSection("World Physics", levelPhysicsProperties);

    //add Level Audio

    

    //add Level Background

	//add Level Conditions(change the name)
	hasScore.removeListener(this);
	hasScore.setValue(var(selectedLevel->isScoreEnabled()));
	BooleanPropertyComponent* scoreFlag = new BooleanPropertyComponent(hasScore, "Scores:", "Use Scoring System");
	
	levelObjConditionalProperties.add(scoreFlag);

	if (selectedLevel->isScoreEnabled()) {
		enemyScore.setValue(var(selectedLevel->getEnemyPoints()));
		SliderPropertyComponent* enemyPointValue = new SliderPropertyComponent(enemyScore, "Enemy Point Value:", 0, 100, 1);
		enemyScore.addListener(this);
		levelObjConditionalProperties.add(enemyPointValue);

		collectableScore.setValue(var(selectedLevel->getCollectablePoints()));
		SliderPropertyComponent* collectablePointValue = new SliderPropertyComponent(collectableScore, "Collectable Point Value:", 0, 100, 1);
		collectableScore.addListener(this);
		levelObjConditionalProperties.add(collectablePointValue);
	}
	hasScore.addListener(this);
	hasTimer.removeListener(this);
	hasTimer.setValue(var(selectedLevel->isTimerEnabled()));
	BooleanPropertyComponent* timerFlag = new BooleanPropertyComponent(hasTimer, "Timer:", "Use Timer");
	levelObjConditionalProperties.add(timerFlag);
	
	if (selectedLevel->isTimerEnabled()) {
		timer.setValue(var(selectedLevel->getTimer()));
		SliderPropertyComponent* timerValue = new SliderPropertyComponent(timer, "Time(seconds):", 0, 500, 1);
		timer.addListener(this);
		levelObjConditionalProperties.add(timerValue);
	}
	hasTimer.addListener(this);


	hasCheckPoint.removeListener(this);
	hasCheckPoint.setValue(var(selectedLevel->isCheckpointEnabled()));
	BooleanPropertyComponent* checkpointFlag = new BooleanPropertyComponent(hasCheckPoint, "Goal Point:", "transition to next level");
	levelObjConditionalProperties.add(checkpointFlag);

	hasCheckPoint.addListener(this);
	propertyPanel.addSection("Conditions", levelObjConditionalProperties);
    propertyPanel.refreshAll();
}

void LevelInspector::textPropertyComponentChanged(TextPropertyComponent * component) {
		if (component->getName() == "Object Name:") {
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
}
void LevelInspector::resized()
{
    juce::Rectangle<int> bounds = getLocalBounds();
    int lineHeight = 50;

    // Play/Pause Button
    playButton.setBounds(bounds.removeFromTop(lineHeight));

	resetLevelButton.setBounds(bounds.removeFromTop(lineHeight));
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
			updateInspectorsChangeBroadcaster->sendChangeMessage();
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
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
		else if (button == &resetLevelButton)
		{
			selectedLevel->resetLevel();
		}
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
    else if (value.refersToSameSourceAs(hasScore)) {
        selectedLevel->setScoreEnabled();
        updateInspectorsChangeBroadcaster->sendChangeMessage();
    }
    else if (value.refersToSameSourceAs(collectableScore)) {
        selectedLevel->setCollectablePoints(value.getValue());
    }
    else if (value.refersToSameSourceAs(enemyScore)) {
        selectedLevel->setEnemyPoints(value.getValue());
    }
    else if (value.refersToSameSourceAs(hasTimer)) {
        selectedLevel->setTimerEnabled();
        updateInspectorsChangeBroadcaster->sendChangeMessage();
    }
    else if (value.refersToSameSourceAs(timer)) {
        selectedLevel->setTimer(value.getValue());
    } 
    else if (value.refersToSameSourceAs(hasCheckPoint)) {
        selectedLevel->setCheckpointEnabled();
        updateInspectorsChangeBroadcaster->sendChangeMessage();
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
}

void LevelInspector::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &levelComboBox)
    {
        // Select Level
        // Update Inspector
        coreEngine->setCurrentLevel(comboBoxThatHasChanged->getSelectedItemIndex());
        //FIX: Removing this stopped an infinite loop from occurring
        updateInspectorsChangeBroadcaster->sendChangeMessage();
    }
}
