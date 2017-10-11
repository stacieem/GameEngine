/*

		Displays a hierarchy of all elements present in the level(scene)
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Level.h"
#include "CoreEngine.h"
#include "Inspector.h"

class LevelInspector : public Component, public InspectorUpdater, public Button::Listener, public TextPropertyComponent::Listener {
public:
	LevelInspector(){
		addAndMakeVisible(propertyPanel);
		addAndMakeVisible(playButton);
		playButton.setButtonText("Start/Stop Game");
		playButton.addListener(this);
	}
	~LevelInspector() {

	}

	void setCoreEngine(CoreEngine* engine) {
		coreEngine = engine;
	}
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		//g.fillAll(Colours::indigo);
	}
	void updateInspector(Level & chosenLevel) {
		propertyPanel.clear();
		levelObjGraphProperties.clear();
		levelPhysicsProperties.clear();
		levelAudioProperties.clear();
		levelBackgroundProperties.clear();
		
		//add object graph
		for (auto gameObj : chosenLevel.getGameObjects()) {
			Value gameObjName;
			gameObjName.setValue(var(gameObj->getName()));
			TextPropertyComponent * objName = new TextPropertyComponent(gameObjName, "Object Name:", 21, false);
			objName->addListener(this);
			levelObjGraphProperties.add(objName);
		}
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);

		//add Level Physics


		Value physicsGravity;
		physicsGravity.setValue(var(-chosenLevel.getWorldPhysics().getGravity()));
		TextPropertyComponent * worldGravity = new TextPropertyComponent(physicsGravity, "Gravity:", 3, false);
		worldGravity->addListener(this);
		levelPhysicsProperties.add(worldGravity);
		propertyPanel.addSection("World Physics", levelPhysicsProperties);
		
		DBG("UPADTED");
		//add Level Audio

		//add Level Background

	}

	void textPropertyComponentChanged(TextPropertyComponent * component) override {

		//Really bad hacky solution since these are generated on the fly right now, they don't exist as member variables
		if (component->getName() == "Gravity:") {
			coreEngine->getGameModel().getCurrentLevel().
				getWorldPhysics().setGravity(0,-component->getText().getFloatValue());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Object Name:") {
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}
	void resized() override
	{
		juce::Rectangle<int> r = getLocalBounds();
		int buttonHeight = r.getHeight() / 20;
		playButton.setBounds(r.getX(),r.getY(),r.getWidth(), buttonHeight);
		propertyPanel.setBounds(r.getX(), buttonHeight, r.getWidth(), buttonHeight * 19);
		//scrollBar.setBounds(0, 0, getWidth()*.2, getHeight());

	}

	void buttonClicked(Button * button) override {
		if (button == &playButton) {
			
			coreEngine->toggleGamePause();
		}
	}

private:
	CoreEngine* coreEngine;
	ToggleButton playButton;
	OwnedArray<TextButton> buttons;
	PropertyPanel propertyPanel;

	Array<PropertyComponent *> levelObjGraphProperties;
	Array<PropertyComponent *> levelPhysicsProperties;
	Array<PropertyComponent *> levelAudioProperties;
	Array<PropertyComponent *> levelBackgroundProperties;
};