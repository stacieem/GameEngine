#include "LevelInspector.h" 

LevelInspector::LevelInspector() : SELECTED_ROW_COLOUR(0xFF4091F5) {
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

    // Update Selected Level Index
    selectedLevelIndex = gameModel.getCurrentLevelIndex();

    // Update Selected Level and Select first Game Object
    if (gameModel.getCurrentLevel() != selectedLevel) {
        selectedLevel = gameModel.getCurrentLevel();
        
        // Deselect old renderable object
        if (selectedObject != nullptr)
            selectedObject->setRenderableIsSelected(false);

        // Select new object
        selectedObject = selectedLevel->getGameObjects().getFirst();
        selectedObject->setRenderableIsSelected(true);
    }
    
    // Clear data to reload with new data
    propertyPanel.clear();
    levelObjGraphProperties.clear();
    levelPhysicsProperties.clear();
    levelAudioProperties.clear();
    levelBackgroundProperties.clear();
    levelComboBox.clear();

    
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
        if (gameObj == selectedObject)
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
    gravity.addListener(this);
    levelPhysicsProperties.add(combo);

    propertyPanel.addSection("World Physics", levelPhysicsProperties);

    //add Level Audio

    

    //add Level Background


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
        coreEngine->removeLevel(selectedLevelIndex);
        updateInspectorsChangeBroadcaster->sendSynchronousChangeMessage();
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
    // Object Selection Button
    else if (value.refersToSameSourceAs(selectedObjectValue))
    {
        // Deselect old renderable object
        if (selectedObject != nullptr)
            selectedObject->setRenderableIsSelected(false);

        // Save new selected object and set it to being rendered as selected
        selectedObject = gameObjects[(int)value.getValue()];
        selectedObject->setRenderableIsSelected(true);
        
        // Update this and other inspectors
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
