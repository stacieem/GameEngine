#include "LevelInspector.h" 

	LevelInspector::LevelInspector() {
		addAndMakeVisible(propertyPanel);
		addAndMakeVisible(playButton);
		playButton.setButtonText("Start/Stop Game");
		playButton.addListener(this);
		selectedObjectValue.addListener(this);
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
	void LevelInspector::updateInspector(Level & chosenLevel) {

		if (&chosenLevel != selectedLevel) {
			selectedLevel = &chosenLevel;
			selectedObject = chosenLevel.getGameObjects().getFirst();
		}
		//propertyPanel.clear();
		propertyPanel.clear();
		levelObjGraphProperties.clear();
		levelPhysicsProperties.clear();
		levelAudioProperties.clear();
		levelBackgroundProperties.clear();


		//add object graph
		for (auto gameObj : chosenLevel.getGameObjects()) {
			gameObjects.addIfNotAlreadyThere(gameObj);
			int index = gameObjects.indexOf(gameObj);
			SelectObjectButtonPropertyComponent* selectButton = new SelectObjectButtonPropertyComponent(index, selectedObjectValue, gameObj->getName(), false);
			levelObjGraphProperties.add(selectButton);
		}
		
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);


		//add Level Physics
		gravity.setValue(var((int)1));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(gravity, "Gravity:");
		combo->setTextWhenNothingSelected("Choose Gravity");
		combo->addItem("High gravity", 3);
		combo->addItem("Anti Gravity", 2);
		combo->addItem("Normal", 1);
		gravity.addListener(this);
		levelPhysicsProperties.add(combo);

		propertyPanel.addSection("World Physics", levelPhysicsProperties);
		//add Level Audio

		//add Level Background
	}

	void LevelInspector::textPropertyComponentChanged(TextPropertyComponent * component) {

		//Really bad hacky solution since these are generated on the fly right now, they don't exist as member variables
		if (component->getName() == "Object Name:") {
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}
	void LevelInspector::resized()
	{
		juce::Rectangle<int> r = getLocalBounds();
		int buttonHeight = r.getHeight() / 20;
		playButton.setBounds(r.getX(), r.getY(), r.getWidth(), buttonHeight);
		propertyPanel.setBounds(r.getX(), buttonHeight, r.getWidth(), buttonHeight * 19);
		//scrollBar.setBounds(0, 0, getWidth()*.2, getHeight());

	}

	void LevelInspector::buttonClicked(Button * button) {
		if (button == &playButton) {

			coreEngine->toggleGamePause();
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}


	void LevelInspector::valueChanged(Value &value) {

		
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
		else
		{
			selectedObject = gameObjects[(int)value.getValue()];
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}

	void LevelInspector::setChildrenEnabled(bool shouldBeEnabled)
	{
		propertyPanel.setEnabled(shouldBeEnabled);
	}

	GameObject* LevelInspector::getSelectedGameObject() {
		return selectedObject;
	}
