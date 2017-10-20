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
			DBG("start");
			selectedObject = chosenLevel.getGameObjects().getFirst();

		}
		//propertyPanel.clear();
		
		levelObjGraphProperties.clear();
		levelPhysicsProperties.clear();
		levelAudioProperties.clear();
		levelBackgroundProperties.clear();

		int size = propertyPanel.getSectionNames().size();

		while (size > 0) {
			propertyPanel.removeSection(size - 1);
			size--;
		}
		//add object graph
		for (auto gameObj : chosenLevel.getGameObjects()) {
			gameObjects.addIfNotAlreadyThere(gameObj);
			int index = gameObjects.indexOf(gameObj);
			SelectObjectButtonPropertyComponent* selectButton = new SelectObjectButtonPropertyComponent(index, selectedObjectValue, gameObj->getName(), false);
			levelObjGraphProperties.add(selectButton);
			
		}
		
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);


		//add Level Physics


		Value physicsGravity;
		physicsGravity.setValue(var(-chosenLevel.getWorldPhysics().getGravity()));
		TextPropertyComponent * worldGravity = new TextPropertyComponent(physicsGravity, "Gravity:", 3, false);
		worldGravity->addListener(this);
		levelPhysicsProperties.add(worldGravity);
		propertyPanel.addSection("World Physics", levelPhysicsProperties);


		propertyPanel.refreshAll();
		//add Level Audio

		//add Level Background

	}

	void LevelInspector::textPropertyComponentChanged(TextPropertyComponent * component) {

		//Really bad hacky solution since these are generated on the fly right now, they don't exist as member variables
		if (component->getName() == "Gravity:") {
			coreEngine->getGameModel().getCurrentLevel().
				getWorldPhysics().setGravity(0, -component->getText().getFloatValue());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

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
		}
	}


	void LevelInspector::valueChanged(Value &value) {
		selectedObject = gameObjects[(int)value.getValue()];
		updateInspectorsChangeBroadcaster->sendChangeMessage();
	}

	GameObject* LevelInspector::getSelectedGameObject() {
		return selectedObject;
	}
