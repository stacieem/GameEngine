/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"
#include "GameObjectType.h"
#include <string>   
#include "ComboBoxPropertyComponent.h"
#include "FilenamePropertyComponent.h"
#include "Speed.h"

class ObjectInspector : public Component, public InspectorUpdater, public TextPropertyComponent::Listener, public Value::Listener, FilenameComponentListener {
public:
	ObjectInspector() {
		//addAndMakeVisible(scrollBar);
		//scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
		addAndMakeVisible(propertyPanel);
		selectedObj = NULL;
	}
	~ObjectInspector() {

	}

	void setCoreEngine(CoreEngine* engine) {
		coreEngine = engine;
	}
	void setSelectedObj(GameObject* obj) {
			
		if (selectedObj != obj) {
			selectedObj = obj;
			updateObj();
			
		}
		
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
	
	void textPropertyComponentChanged(TextPropertyComponent * component) override{

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


		if (component->getName() == "Density:") {
			float dens = component->getText().getFloatValue()/100;
			selectedObj->getPhysicsProperties().setDensity(dens);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

	}

	void valueChanged(Value &value) {
		if (value.refersToSameSourceAs(xPosition)) {
			if (value.getValue().isDouble()) {
				
				float x = (float)value.getValue();
				selectedObj->setXPositionWithPhysics(x);
				updateInspectorsChangeBroadcaster->sendChangeMessage();

			}

		}
		if (value.refersToSameSourceAs(objPhysicsXCap)) {
			switch ((int)objPhysicsXCap.getValue()) {
			case 1:
				selectedObj->setXVelocityCap(Speed::SLOW);
				break;
			case 2:
				selectedObj->setXVelocityCap(Speed::MED);
				break;
			case 3:
				selectedObj->setXVelocityCap(Speed::FAST);
				break;
			}

		}

		if (value.refersToSameSourceAs(objPhysicsYCap)) {
			switch ((int)objPhysicsYCap.getValue()) {
			case 1:
				selectedObj->setYVelocityCap(Speed::SLOW);
				break;
			case 2:
				selectedObj->setYVelocityCap(Speed::MED);
				break;
			case 3:
				selectedObj->setYVelocityCap(Speed::FAST);
				break;
			}

		}

		if (value.refersToSameSourceAs(yPosition)) {
			if (value.getValue().isDouble()) {
				

				float y = (float)value.getValue();
				selectedObj->setYPositionWithPhysics(y);
				updateInspectorsChangeBroadcaster->sendChangeMessage();

			}

		}

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
				selectedObj->updateState(GameObject::STATIC);
				break;
			case 2:
				selectedObj->updateState(GameObject::DYNAMIC);
				break;
			}
		}

		if (value.refersToSameSourceAs(aiState)) {

			switch ((int)aiState.getValue()) {
			case 1:
				dynamic_cast<EnemyObject *>(selectedObj)->changeAI(EnemyObject::CHASE);
				break;
			case 2:
				dynamic_cast<EnemyObject *>(selectedObj)->changeAI(EnemyObject::SCAREDAF);
				break;
			}
		}

		if (value.refersToSameSourceAs(objPhysicsFriction)) {
			float fric = (float)value.getValue();
			selectedObj->getPhysicsProperties().setFriction(fric);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (value.refersToSameSourceAs(objPhysicsRestitution)) {

			float rest = (float)value.getValue();
			selectedObj->getPhysicsProperties().setRestitution(rest);
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
		
	}

private:

	void updateObj() {
		propertyPanel.clear();
		objPhysicsProperties.clear();
		objAudioProperties.clear();
		objBackgroundProperties.clear();

		//add Level Physics
		//create additional functions in desired objects to make it easier to retrieve information.
		//add sections to physics menu
		if (selectedObj != NULL) {
			switch (selectedObj->getObjType()) {
			case Generic:	//environment?
				addGenericMovementProperties();
				break;
			case Player:	//player
				addGenericMovementProperties();
				break;
			case Enemy:	//ai, maybe differntiate between types of ai with this?
				aiState.setValue(var((int)1));
				ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(aiState, "Ai Mode:");
				combo->setTextWhenNothingSelected("Choose Ai Type");
				combo->addItem("Flee", 2);
				combo->addItem("Chase", 1);
				aiState.addListener(this);
				objPhysicsProperties.add(combo);
				break;

			
			}

			addGenericGraphicProperties();
			//add to panel
			propertyPanel.addSection("Object Physics", objPhysicsProperties);

			//Add misc properties to panel
			propertyPanel.addSection("Misc. Properties", objBackgroundProperties);

		}

		//add Object Audio

	}


	//base physics properties
	void addGenericMovementProperties() {

		objPhysicsXCap.setValue(var((int)1));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(objPhysicsXCap, "Move Speed:");
		combo->setTextWhenNothingSelected("Choose Move Speed");
		combo->addItem("High", 3);
		combo->addItem("Med", 2);
		combo->addItem("Low", 1);
		objPhysicsXCap.addListener(this);
		objPhysicsProperties.add(combo);

		objPhysicsYCap.setValue(var((int)1));
		combo = new ComboBoxPropertyComponent(objPhysicsYCap, "Jump Speed:");
		combo->setTextWhenNothingSelected("Choose Jump Speed");
		combo->addItem("High", 3);
		combo->addItem("Med", 2);
		combo->addItem("Low", 1);
		objPhysicsYCap.addListener(this);
		objPhysicsProperties.add(combo);

		objPhysicsFriction.setValue(var(selectedObj->getPhysicsProperties().getFriction()));
		SliderPropertyComponent* objFrictionText = new SliderPropertyComponent(objPhysicsFriction, "Friction:", 0, 1.0, 0.1);
		objPhysicsFriction.addListener(this);
		objPhysicsProperties.add(objFrictionText);

		objPhysicsRestitution.setValue(var(selectedObj->getPhysicsProperties().getRestitution()));
		SliderPropertyComponent* objRestitutionText = new SliderPropertyComponent(objPhysicsRestitution, "Bounciness:", 0, 10.0, 0.1);
		objPhysicsRestitution.addListener(this);
		objPhysicsProperties.add(objRestitutionText);

		stateComboValue.setValue(var((int)1));
		combo = new ComboBoxPropertyComponent(stateComboValue, "State:");
		combo->setTextWhenNothingSelected("Choose State");
		combo->addItem("Dynamic", 2);
		combo->addItem("Static", 1);
		stateComboValue.addListener(this);
		objPhysicsProperties.add(combo);

		//Density removed for now
		/*objPhysicsDensity.setValue(var(selectedObj->getPhysicsProperties().getDensity()));
		TextPropertyComponent* objDensityText = new TextPropertyComponent(objPhysicsDensity, "Density:", 3, false);
		objDensityText->addListener(this);
		objPhysicsProperties.add(objDensityText);*/

	}

	//base Graphical properties
	void addGenericGraphicProperties() {



		//Set objName to be the name of the selected object, and create its TextPropertyComponent
		objName.setValue(var(selectedObj->getName()));
		TextPropertyComponent* objNameText = new TextPropertyComponent(objName, "Name:", 40, false);
		objNameText->addListener(this);

		objBackgroundProperties.add(objNameText);

		//This is going to require a specification of the axis for the game
		xPosition.setValue(var((int)selectedObj->getRenderableObject().position.x));
		xPosition.addListener(this);
		SliderPropertyComponent* slider = new SliderPropertyComponent(xPosition, "x-Position:", -10, 10, .25);

		//new TextPropertyComponent(xPosition, "x-Position:", 4,false);

		objBackgroundProperties.add(slider);

		//This is going to require a specification of the axis for the game
		yPosition.setValue(var((int)selectedObj->getRenderableObject().position.y));
		SliderPropertyComponent* slider2 = new SliderPropertyComponent(yPosition, "y-Position:", -10, 10, .25);
		yPosition.addListener(this);
		objBackgroundProperties.add(slider2);

		//Note that this is the custom ComboBoxPropertyComponent JUCE docs
		comboValue.setValue(var((int)1));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(comboValue, "Animation Speed:");
		combo->setTextWhenNothingSelected("Choose Speed");
		combo->addItem("Fast", 3);
		combo->addItem("Normal", 2);
		combo->addItem("Slow", 1);
		comboValue.addListener(this);
		objBackgroundProperties.add(combo);

		FilenamePropertyComponent* filename = new FilenamePropertyComponent("Choose Idle Texture", selectedObj->getRenderableObject().animationProperties.getIdleTexture(), false, false, false, "", "", "Select a file");
		filename->addListener(this);

		objBackgroundProperties.add(filename);

		FilenamePropertyComponent* animationDirectory = new FilenamePropertyComponent("Animation Directory", selectedObj->getRenderableObject().animationProperties.getAnimationTextureDirectory(), false, true, false, "", "", "Select a Dir");
		animationDirectory->addListener(this);
		objBackgroundProperties.add(animationDirectory);
	}

	CoreEngine* coreEngine;
	GameObject* selectedObj;
	//Slider scrollBar;
	//selected object properties
	PropertyPanel propertyPanel;
	Array<PropertyComponent *> objPhysicsProperties;
	Array<PropertyComponent *> objAudioProperties;
	Array<PropertyComponent *> objBackgroundProperties;

	Value objTexture, objName, xPosition, yPosition,
		  objPhysicsX, objPhysicsY, objPhysicsXCap, objPhysicsYCap,
		  objPhysicsFriction, objPhysicsRestitution, objPhysicsDensity,
	      comboValue, stateComboValue, aiState;

	ScopedPointer<FilenameComponent> chooseFile;

};
