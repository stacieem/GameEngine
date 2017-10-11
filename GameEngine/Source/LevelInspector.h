/*

		Displays a hierarchy of all elements present in the level(scene)
*/
#pragma once
#include "Level.h"
#include "CoreEngine.h"
#include "Inspector.h"

class LevelInspector : public Component, public InspectorUpdater,
                       public Button::Listener, public ComboBox::Listener
{
public:
	LevelInspector(){
		addAndMakeVisible(propertyPanel);
		addAndMakeVisible(playButton);
        addAndMakeVisible(levelLabel);
        addAndMakeVisible(levelComboBox);
        addAndMakeVisible(addLevelButton);
        addAndMakeVisible(removeLevelButton);
        
        addLevelButton.setButtonText("+");
        removeLevelButton.setButtonText("-");
        
        levelLabel.setJustificationType(Justification::centred);
        
		playButton.setButtonText("Start/Stop Game");
		playButton.addListener(this);
        addLevelButton.addListener(this);
        removeLevelButton.addListener(this);
        levelComboBox.addListener(this);
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
	void updateInspector(GameModel & gameModel) {
		propertyPanel.clear();
		levelObjGraphProperties.clear();
		levelPhysicsProperties.clear();
		levelAudioProperties.clear();
		levelBackgroundProperties.clear();
        
        Level * currentLevel = gameModel.getCurrentLevel();
        currentLevelIndex = gameModel.getCurrentLevelIndex();
        levelComboBox.clear();
        
        // Set the current level text
        levelLabel.setText("Level: ", NotificationType::dontSendNotification);
        
        // For the number of levels, add a level to the comboBox selector
        for (int i = 0; i < gameModel.getNumLevels(); ++i)
        {
            levelComboBox.addItem(String(i + 1), i + 1);
            
            // If the current index is the level, make it the selected level
            if (i == currentLevelIndex) {
                levelComboBox.setSelectedItemIndex(i, NotificationType::dontSendNotification);
            }
        }
		
		//add object graph
		for (auto gameObj : currentLevel->getGameObjects()) {
			Value gameObjName;
			gameObjName.setValue(var(gameObj->getName()));
			levelObjGraphProperties.add(new TextPropertyComponent(gameObjName,"Object Name:",21,false));
		}
		propertyPanel.addSection("Object Graph", levelObjGraphProperties);

		//add Level Physics
		Value physicsGravity;
		physicsGravity.setValue(var(-currentLevel->getWorldPhysics().getGravity()));
		levelPhysicsProperties.add(new SliderPropertyComponent(physicsGravity, "Gravity:", -20.0,20.0, 0.1));
		propertyPanel.addSection("World Physics", levelPhysicsProperties);
        
		//add Level Audio

		//add Level Background

	}
	void resized() override
	{
		juce::Rectangle<int> bounds = getLocalBounds();
        int lineHeight = 50;
        
        // Play/Pause Button
		playButton.setBounds(bounds.removeFromTop(lineHeight));
        
        // Level Selection
        juce::Rectangle<int> levelSelectRow = bounds.removeFromTop(lineHeight);
        
        Font levelLabelFont;
        levelLabelFont.setHeight(levelSelectRow.getHeight()-20);
        levelLabel.setFont(levelLabelFont);
        levelLabel.setBounds(levelSelectRow.removeFromLeft(levelSelectRow.getWidth() / 2));
        levelComboBox.setBounds(levelSelectRow.removeFromLeft(levelSelectRow.getWidth() * 2 / 3));
        addLevelButton.setBounds(levelSelectRow.removeFromTop(levelSelectRow.getHeight() / 2));
        removeLevelButton.setBounds(levelSelectRow);
        
        
        // Property Panels
		propertyPanel.setBounds(bounds);

	}

	void buttonClicked(Button * button) override {
		if (button == &playButton)
        {
			coreEngine->toggleGamePause();
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
    
    void comboBoxChanged (ComboBox *comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == &levelComboBox)
        {
            // Select Level
            // Update Inspector
            coreEngine->setCurrentLevel(comboBoxThatHasChanged->getSelectedItemIndex());
            updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
        }
    }

private:
    
    Label levelLabel;
    ComboBox levelComboBox;
    TextButton addLevelButton;
    TextButton removeLevelButton;
    int currentLevelIndex;
    
	CoreEngine* coreEngine;
	ToggleButton playButton;
	OwnedArray<TextButton> buttons;
	PropertyPanel propertyPanel;

	Array<PropertyComponent *> levelObjGraphProperties;
	Array<PropertyComponent *> levelPhysicsProperties;
	Array<PropertyComponent *> levelAudioProperties;
	Array<PropertyComponent *> levelBackgroundProperties;
};
