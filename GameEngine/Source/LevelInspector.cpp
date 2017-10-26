#include "LevelInspector.h" 

	LevelInspector::LevelInspector() {
		addAndMakeVisible(propertyPanel);
		addAndMakeVisible(playButton);
		playButton.setButtonText("Start/Stop Game");
		playButton.addListener(this);
		selectedObjectValue.addListener(this);


		addAndMakeVisible(levelLabel);
		addAndMakeVisible(levelComboBox);
		addAndMakeVisible(addLevelButton);
		addAndMakeVisible(removeLevelButton);

		addLevelButton.setButtonText("+");
		removeLevelButton.setButtonText("-");

		levelLabel.setJustificationType(Justification::centred);

		addLevelButton.addListener(this);
		removeLevelButton.addListener(this);
		levelComboBox.addListener(this);
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
	void LevelInspector::updateInspector(GameModel & gameModel) {

		Level * currentLevel = gameModel.getCurrentLevel();
		currentLevelIndex = gameModel.getCurrentLevelIndex();

		if (currentLevel != selectedLevel) {
			selectedLevel = currentLevel;
			selectedObject = currentLevel->getGameObjects().getFirst();
			
		} 
		
		//propertyPanel.clear();
		
		levelObjGraphProperties.clear();
		levelPhysicsProperties.clear();
		levelAudioProperties.clear();
		levelBackgroundProperties.clear();

		
		levelComboBox.clear();

		// Set the current level text
		levelLabel.setText("Level: ", NotificationType::dontSendNotification);
		DBG(gameModel.getNumLevels());

		int numLevels = gameModel.getNumLevels();

		// For the number of levels, add a level to the comboBox selector
		for (int i = 0; i < numLevels; ++i)
		{
			
			levelComboBox.addItem(String(i + 1), i + 1);

			// If the current index is the level, make it the selected level
			if (i == currentLevelIndex) {
				levelComboBox.setSelectedItemIndex(i, NotificationType::dontSendNotification);
			}
		}
		

		int size = propertyPanel.getSectionNames().size();

		while (size > 0) {
			propertyPanel.removeSection(size - 1);
			size--;
		}
		//add object graph
		for (auto gameObj : currentLevel->getGameObjects()) {
			gameObjects.addIfNotAlreadyThere(gameObj);
			int index = gameObjects.indexOf(gameObj);
			SelectObjectButtonPropertyComponent* selectButton = new SelectObjectButtonPropertyComponent(index, selectedObjectValue, gameObj->getName(), false);
			levelObjGraphProperties.add(selectButton);
			
		}
		
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);


		//add Level Physics


		Value physicsGravity;
		physicsGravity.setValue(var(-currentLevel->getWorldPhysics().getGravity()));
		TextPropertyComponent * worldGravity = new TextPropertyComponent(physicsGravity, "Gravity:", 3, false);
		worldGravity->addListener(this);
		levelPhysicsProperties.add(worldGravity);
		propertyPanel.addSection("World Physics", levelPhysicsProperties);


		/* //add Level Physics
		Value physicsGravity;
		physicsGravity.setValue(var(-currentLevel->getWorldPhysics().getGravity()));
		levelPhysicsProperties.add(new SliderPropertyComponent(physicsGravity, "Gravity:", -20.0, 20.0, 0.1));
		propertyPanel.addSection("World Physics", levelPhysicsProperties);*/

		//add Level Audio

		

		//add Level Background

		propertyPanel.refreshAll();
		
	}

	void LevelInspector::textPropertyComponentChanged(TextPropertyComponent * component) {

		//Really bad hacky solution since these are generated on the fly right now, they don't exist as member variables
		if (component->getName() == "Gravity:") {
			
			coreEngine->getGameModel().getCurrentLevel()->
				getWorldPhysics().setGravity(0, -component->getText().getFloatValue());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Object Name:") {
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}
	void LevelInspector::resized()
	{
		/*juce::Rectangle<int> r = getLocalBounds();
		int buttonHeight = r.getHeight() / 20;
		playButton.setBounds(r.getX(), r.getY(), r.getWidth(), buttonHeight);
		propertyPanel.setBounds(r.getX(), buttonHeight, r.getWidth(), buttonHeight * 19);
		//scrollBar.setBounds(0, 0, getWidth()*.2, getHeight());*/


		juce::Rectangle<int> bounds = getLocalBounds();
		int lineHeight = 50;

		// Play/Pause Button
		playButton.setBounds(bounds.removeFromTop(lineHeight));

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
			coreEngine->removeLevel(currentLevelIndex);
			updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
	}


	void LevelInspector::valueChanged(Value &value) {
		selectedObject = gameObjects[(int)value.getValue()];
		updateInspectorsChangeBroadcaster->sendChangeMessage();
	}

	void LevelInspector::setChildrenEnabled(bool shouldBeEnabled)
	{
		propertyPanel.setEnabled(shouldBeEnabled);
	}

	GameObject* LevelInspector::getSelectedGameObject() {
		return selectedObject;
	}

	void LevelInspector::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
	{
		if (comboBoxThatHasChanged == &levelComboBox)
		{
			// Select Level
			// Update Inspector
			coreEngine->setCurrentLevel(comboBoxThatHasChanged->getSelectedItemIndex());
			
			//FIX: Removing this stopped an infinite loop from occurring
			//updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
		}
	}
