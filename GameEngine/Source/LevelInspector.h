/*

		Displays a hierarchy of all elements present in the level(scene)
*/
#pragma once
#include "Level.h";
class LevelInspector : public Component {
public:
	LevelInspector() {
		addAndMakeVisible(propertyPanel);
	}
	~LevelInspector() {

	}

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		//g.fillAll(Colours::indigo);
	}
	void updateInspector(Level & chosenLevel) {
		propertyPanel.clear();
		
		//add object graph
		for (auto gameObj : chosenLevel.getGameObjects()) {
			Value gameObjName;
			gameObjName.setValue(var(gameObj->getName()));
			levelObjGraphProperties.add(new TextPropertyComponent(gameObjName,"Object Name:",21,false));
		}
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);

		//add Level Physics


		Value physicsGravity;
		physicsGravity.setValue(var(-chosenLevel.getWorldPhysics().getGravity()));
		levelPhysicsProperties.add(new SliderPropertyComponent(physicsGravity, "Gravity:", -20.0,20.0, 0.1));
		propertyPanel.addSection("World Physics", levelPhysicsProperties);
		
		//add Level Audio

		//add Level Background

	}
	void resized() override
	{
		//juce::Rectangle<int> r = getLocalBounds();
		propertyPanel.setBounds(getLocalBounds());
		//scrollBar.setBounds(0, 0, getWidth()*.2, getHeight());

	}
private:
	TextButton txt;
	OwnedArray<TextButton> buttons;
	PropertyPanel propertyPanel;

	Array<PropertyComponent *> levelObjGraphProperties;
	Array<PropertyComponent *> levelPhysicsProperties;
	Array<PropertyComponent *> levelAudioProperties;
	Array<PropertyComponent *> levelBackgroundProperties;
	// Game Data Model
	//OwnedArray<GameObject> gameObjects;
};