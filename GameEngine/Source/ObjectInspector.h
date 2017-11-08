/*

		Lists the properties of the currently selected unit
*/
#pragma once

#include "CoreEngine.h"
#include "InspectorUpdater.h"
#include "GameObjectType.h"
#include <string>   
#include "ComboBoxPropertyComponent.h"
#include "FilenamePropertyComponent.h"
#include "Speed.h"

class ObjectInspector : public Component, public InspectorUpdater, 
						public TextPropertyComponent::Listener, 
						public Value::Listener, FilenameComponentListener {
public:
	ObjectInspector() {
		//addAndMakeVisible(scrollBar);
		//scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
		addAndMakeVisible(propertyPanel);
		levelToWin.addListener(this);
		selectedObj = NULL;
	}
	~ObjectInspector() {

	}

	void setCoreEngine(CoreEngine* engine) {
		coreEngine = engine;
	}
	void setSelectedObj(GameObject* obj) {
			
		//if (selectedObj != obj) {
			selectedObj = obj;
			updateObj();
			
		//}
		
	}
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		//g.fillAll(Colours::coral);
		//g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	}
	
	void resized() override
	{
		//scrollBar.setBounds(getLocalBounds());

		propertyPanel.setBounds(getLocalBounds());
	}
	
	void textPropertyComponentChanged(TextPropertyComponent * component) override
    {
		if (component->getName() == "Name:") {
			selectedObj->setName(component->getText());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Texture:") {

			File textureFile;
			if (component->getText().isEmpty()) {
				textureFile = File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/default.png");
			} else {
				textureFile = File(component->getText());
			}
			selectedObj->getRenderableObject().animationProperties.setIdleTexture(textureFile);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		/*
		if (component->getName() == "Density:") {
			float dens = component->getText().getFloatValue()/100;
			selectedObj->getPhysicsProperties().setDensity(dens);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		*/
	}

	void valueChanged(Value &value) override
    {
		if (value.refersToSameSourceAs(objPhysicsXCap)) {
			switch ((int)objPhysicsXCap.getValue()) {
			case 1:
				selectedObj->setMoveSpeed(Speed::SLOW);
				break;
			case 2:
				selectedObj->setMoveSpeed(Speed::MED);
				break;
			case 3:
				selectedObj->setMoveSpeed(Speed::FAST);
				break;
			}

		}

		if (value.refersToSameSourceAs(objPhysicsYCap)) {
			switch ((int)objPhysicsYCap.getValue()) {
			case 1:
				selectedObj->setJumpSpeed(Speed::SLOW);
				break;
			case 2:
				selectedObj->setJumpSpeed(Speed::MED);
				break;
			case 3:
				selectedObj->setJumpSpeed(Speed::FAST);
				break;
			}

		}
		/*
		if (value.refersToSameSourceAs(yPosition)) {
			if (value.getValue().isDouble()) {
				

				float y = (float)value.getValue();
				selectedObj->setYPositionWithPhysics(y);
				updateInspectorsChangeBroadcaster->sendChangeMessage();

			}

		}
		*/
		if (value.refersToSameSourceAs(comboValue)) {
			
			switch ((int)comboValue.getValue()) {
			case 1:
				selectedObj->getRenderableObject().animationProperties.setAnimationSpeed(Speed::SLOW);
				break;
			case 2:
				selectedObj->getRenderableObject().animationProperties.setAnimationSpeed(Speed::MED);
				break;
			case 3:
				selectedObj->getRenderableObject().animationProperties.setAnimationSpeed(Speed::FAST);
				break;
			}
		}

		if (value.refersToSameSourceAs(stateComboValue)) {

			switch ((int)stateComboValue.getValue()) {
			case 1:
                selectedObj->getPhysicsProperties().setIsStatic(true);
				break;
			case 2:
                selectedObj->getPhysicsProperties().setIsStatic(false);
				break;
			}
		}

		if (value.refersToSameSourceAs(aiState)) {

			switch ((int)aiState.getValue()) {
			case 1:
				((EnemyObject*)(selectedObj))->changeAI(EnemyObject::NONE);
				break;
			case 2:
				((EnemyObject*)(selectedObj))->changeAI(EnemyObject::GROUNDPATROL);
				break;
			case 3:
				((EnemyObject*)(selectedObj))->changeAI(EnemyObject::JUMPPATROL);
				break;
			case 4:
				((EnemyObject*)(selectedObj))->changeAI(EnemyObject::SCAREDAF);
				break;
			case 5:
				((EnemyObject*)(selectedObj))->changeAI(EnemyObject::CHASE);
				break;
			}
		}else if (value.refersToSameSourceAs(playerLives)) {
			selectedObj->setLives(value.getValue());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}else if (value.refersToSameSourceAs(levelToWin)) {
			((GoalPointObject*)selectedObj)->setToWin();
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}else if (value.refersToSameSourceAs(levelGoTo)) {
			((GoalPointObject*)selectedObj)->setLevelToGoTo(levelGoTo.getValue());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}

	void filenameComponentChanged(FilenameComponent *fileComponentThatHasChanged) {
		if (fileComponentThatHasChanged->getName() == "Animation Directory") {
			selectedObj->getRenderableObject().animationProperties.setAnimationTextures(fileComponentThatHasChanged->getCurrentFile());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (fileComponentThatHasChanged->getName() == "Choose Idle Texture") {
			selectedObj->getRenderableObject().animationProperties.setIdleTexture(fileComponentThatHasChanged->getCurrentFile());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
        
        if (fileComponentThatHasChanged->getName() == "Choose Collision Audio") {
            selectedObj->mapAudioFileToPhysicalAction(fileComponentThatHasChanged->getCurrentFile(), PhysicalAction::collsion);
            updateInspectorsChangeBroadcaster->sendChangeMessage();
        }
		
	}

private:

	void updateObj()
    {
		propertyPanel.clear();
		objPhysicsProperties.clear();
		objAudioProperties.clear();
		objBackgroundProperties.clear();
		objHudProperties.clear();

		//add Level Physics
		//create additional functions in desired objects to make it easier to retrieve information.
		//add sections to physics menu
		if (selectedObj != NULL)
        {
			switch (selectedObj->getObjType())
            {
			case Generic:	//environment?
				addGenericMovementProperties();
				break;
			case Player:	//player
				addGenericMovementProperties();
				addHudProperties();

				//add to panel
				propertyPanel.addSection("Object Physics", objPhysicsProperties);
				break;
			case Enemy:	//ai, maybe differntiate between types of ai with this?
				addGenericMovementProperties();
				addEnemyProperties();

				//add to panel
				propertyPanel.addSection("Object Physics", objPhysicsProperties);
                break;
			case Checkpoint:
				addGenericMovementProperties();
				addGoalPointProperties();

				propertyPanel.addSection("Object Physics", objPhysicsProperties);
				
				break;
			case Collectable:
				break;
			}

			addGenericGraphicProperties();
            
            addAudioProperties();
            

			//Add misc properties to panel
			propertyPanel.addSection("Misc. Properties", objBackgroundProperties);
            
            // Add Object Audio
            propertyPanel.addSection("Audio", objAudioProperties);
		}

		//add Object Audio

	}

	void addGoalPointProperties() {


		levelToWin.removeListener(this);
		levelToWin.setValue(var(((GoalPointObject*)selectedObj)->getToWin()));
		BooleanPropertyComponent* toWinFlag = new BooleanPropertyComponent(levelToWin, "Transition", "Win Game?");
		objBackgroundProperties.add(toWinFlag);
		
		if (!((GoalPointObject*)selectedObj)->getToWin()) {
			GoalPointObject* goal = ((GoalPointObject*)selectedObj);
			levelGoTo.setValue(var(goal->getLevelToGoTo()));
			ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(levelGoTo, "Next Level:");
			combo->setTextWhenNothingSelected("Choose next Level");
			combo->addItem("1", 1);
			for (int i = 1; i < coreEngine->getGameModel().getNumLevels(); i++) {
				combo->addItem(String(i + 1), i + 1);
			}
			combo->setSelectedId(goal->getLevelToGoTo(), NotificationType::dontSendNotification);
			objBackgroundProperties.add(combo);
		}
		
		levelToWin.addListener(this);
		levelGoTo.addListener(this);

	}
	
	void addEnemyProperties() {
		aiState.setValue(var(((EnemyObject*)selectedObj)->getAIState()));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(aiState, "AI:");
		combo->setTextWhenNothingSelected("Choose AI Type");
		combo->addItem("Chase", 5);
		combo->addItem("Flee", 4);
		combo->addItem("Jump & Patrol", 3);
		combo->addItem("Patrol", 2);
		combo->addItem("Do Nothing", 1);
		combo->setSelectedId(((EnemyObject*)selectedObj)->getAIState() + 1, dontSendNotification);
		aiState.addListener(this);
		objBackgroundProperties.add(combo);
	}
	//base physics properties
	void addGenericMovementProperties() {

		objPhysicsXCap.setValue(var(selectedObj->getMoveSpeed()));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(objPhysicsXCap, "Move Speed:");
		combo->setTextWhenNothingSelected("Choose Move Speed");
		combo->addItem("Fast", 3);
		combo->addItem("Medium", 2);
		combo->addItem("Slow", 1);
		combo->setSelectedId(selectedObj->getMoveSpeed() + 1, dontSendNotification);
		objPhysicsXCap.addListener(this);
		objPhysicsProperties.add(combo);

		objPhysicsYCap.setValue(var(selectedObj->getJumpSpeed()));
		combo = new ComboBoxPropertyComponent(objPhysicsYCap, "Jump Speed:");
		combo->setTextWhenNothingSelected("Choose Jump Speed");
		combo->addItem("Fast", 3);
		combo->addItem("Medium", 2);
		combo->addItem("Slow", 1);
		combo->setSelectedId(selectedObj->getJumpSpeed()+1, dontSendNotification);
		objPhysicsYCap.addListener(this);
		objPhysicsProperties.add(combo);
		/*
		objPhysicsFriction.setValue(var(selectedObj->getPhysicsProperties().getFriction()));
		SliderPropertyComponent* objFrictionText = new SliderPropertyComponent(objPhysicsFriction, "Friction:", 0, 1.0, 0.1);
		objPhysicsFriction.addListener(this);
		objPhysicsProperties.add(objFrictionText);

		objPhysicsRestitution.setValue(var(selectedObj->getPhysicsProperties().getRestitution()));
		SliderPropertyComponent* objRestitutionText = new SliderPropertyComponent(objPhysicsRestitution, "Bounciness:", 0, 10.0, 0.1);
		objPhysicsRestitution.addListener(this);
		objPhysicsProperties.add(objRestitutionText);
		*/
		stateComboValue.setValue(var(selectedObj->getPhysicsProperties().getIsStatic()));
		combo = new ComboBoxPropertyComponent(stateComboValue, "Physics:");
		combo->setTextWhenNothingSelected("Choose if active");
        combo->addItem("No", 1);
        combo->addItem("Yes", 2);
        combo->setSelectedId(selectedObj->getPhysicsProperties().getIsStatic() ? 1 : 2, NotificationType::dontSendNotification);
		stateComboValue.addListener(this);
		objPhysicsProperties.add(combo);

	}

	//base HUD properties
	void addHudProperties()
    {
			playerLives.setValue(var(selectedObj->getLives()));
			SliderPropertyComponent* playerHealthValue = new SliderPropertyComponent(playerLives, "Lives:", 0, 100, 1);
			playerLives.addListener(this);
			objHudProperties.add(playerHealthValue);
		
		//Add HUD properties to panel
		propertyPanel.addSection("HUD Properties", objHudProperties);
	}
                            
                            
	//base Graphical properties
	void addGenericGraphicProperties()
    {
		//Set objName to be the name of the selected object, and create its TextPropertyComponent
		objName.setValue(var(selectedObj->getName()));
		TextPropertyComponent* objNameText = new TextPropertyComponent(objName, "Name:", 40, false);
		objNameText->addListener(this);

		objBackgroundProperties.add(objNameText);
		/*
		//This is going to require a specification of the axis for the game
		xPosition.setValue(var((int)selectedObj->getRenderableObject().position.x));
		xPosition.addListener(this);
		SliderPropertyComponent* slider = new SliderPropertyComponent(xPosition, "x-Position:", -10, 10, .25);

		//new TextPropertyComponent(xPosition, "x-Position:", 4,false);

		//objBackgroundProperties.add(slider);

		//This is going to require a specification of the axis for the game
		yPosition.setValue(var((int)selectedObj->getRenderableObject().position.y));
		SliderPropertyComponent* slider2 = new SliderPropertyComponent(yPosition, "y-Position:", -10, 10, .25);
		yPosition.addListener(this);
		//objBackgroundProperties.add(slider2);
		*/
		//Note that this is the custom ComboBoxPropertyComponent JUCE docs
		comboValue.setValue(var((selectedObj->getRenderableObject()).animationProperties.getAnimationSpeed()));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(comboValue, "Animation Speed:");
		combo->setTextWhenNothingSelected("Choose Speed");
		combo->addItem("Fast", 3);
		combo->addItem("Normal", 2);
		combo->addItem("Slow", 1);
		combo->setSelectedId((selectedObj->getRenderableObject()).animationProperties.getAnimationSpeed() + 1, dontSendNotification);
		comboValue.addListener(this);
		objBackgroundProperties.add(combo);

		FilenamePropertyComponent* filename = new FilenamePropertyComponent("Choose Idle Texture", selectedObj->getRenderableObject().animationProperties.getIdleTexture(), false, false, false, "", "", "Select a file");
		filename->addListener(this);
		objBackgroundProperties.add(filename);

		FilenamePropertyComponent* animationDirectory = new FilenamePropertyComponent("Animation Directory", selectedObj->getRenderableObject().animationProperties.getAnimationTextureDirectory(), false, true, false, "", "", "Select a Dir");
		animationDirectory->addListener(this);
		objBackgroundProperties.add(animationDirectory);
	}

                            
    void addAudioProperties()
    {
        // Get file already associated with selected object
        File * collisionAudioFile = selectedObj->getAudioFileForAction(PhysicalAction::collsion);
        
        FilenamePropertyComponent* collisionAudio = new FilenamePropertyComponent("Choose Collision Audio", (collisionAudioFile == nullptr) ? File() : *collisionAudioFile, false, false, false, "", "", "Select a file");
        collisionAudio->addListener(this);
        objAudioProperties.add(collisionAudio);
    }

	CoreEngine* coreEngine;
	GameObject* selectedObj;
	//Slider scrollBar;
	//selected object properties
	PropertyPanel propertyPanel;
	Array<PropertyComponent *> objPhysicsProperties;
	Array<PropertyComponent *> objAudioProperties;
	Array<PropertyComponent *> objBackgroundProperties;
	Array<PropertyComponent *> objHudProperties;

	Value objName, objPhysicsX, objPhysicsY, objPhysicsXCap, objPhysicsYCap,
		   objPhysicsDensity, comboValue, stateComboValue, aiState,
			playerLives, levelGoTo,levelToWin;

	ScopedPointer<FilenameComponent> chooseFile;

};
