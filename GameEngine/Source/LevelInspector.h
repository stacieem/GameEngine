/*

		Displays a hierarchy of all elements present in the level(scene)
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Level.h"
#include "CoreEngine.h"
#include "InspectorUpdater.h"
#include "SelectObjectButtonPropertyComponent.h"
#include "ComboBoxPropertyComponent.h"
#include "WorldNavigator.h"

class LevelInspector : public Component, public InspectorUpdater,
                       public Button::Listener, public ComboBox::Listener, 
					   public TextPropertyComponent::Listener, public Value::Listener						
{
public:
	LevelInspector(WorldNavigator & worldNavigator);

	~LevelInspector();

	void setCoreEngine(CoreEngine* engine);
	// JUCE GUI Callbacks ======================================================

	void updateInspector(GameModel & gameModel);
	void paint(Graphics& g) override;

	void textPropertyComponentChanged(TextPropertyComponent * component) override;

	void resized() override;


	void buttonClicked(Button * button) override;
    
	void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;


	void valueChanged(Value &value) override;

	void setChildrenEnabled(bool shouldBeEnabled);

private:
    
    const Colour SELECTED_ROW_COLOUR;
    
    Label levelLabel;
    ComboBox levelComboBox;
    TextButton addLevelButton;
    TextButton removeLevelButton;
	TextButton resetLevelButton;
    
	CoreEngine* coreEngine;
	ToggleButton playButton;
	OwnedArray<TextButton> buttons;
	PropertyPanel propertyPanel;
	Value selectedObjectValue, gravity, endCheckPoint, hasScore;
	Value hasTimer, timer;
	Value hasCheckPoint;
	Value enemyScore, collectableScore;
	Array<GameObject*> gameObjects;

    // Level selection
	Level* selectedLevel;
    int selectedLevelIndex;
    
    WorldNavigator & worldNavigator;

	Array<PropertyComponent *> levelObjGraphProperties;
	Array<PropertyComponent *> levelPhysicsProperties;
	Array<PropertyComponent *> levelAudioProperties;
	Array<PropertyComponent *> levelBackgroundProperties;
	Array<PropertyComponent *> levelObjConditionalProperties;
};
