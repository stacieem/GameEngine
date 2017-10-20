/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"
#include "GameObjectType.h"
#include <string>   
#include <string>   
#include "ComboBoxPropertyComponent.h"
#include "FilenamePropertyComponent.h"

class ObjectInspector : public Component, public InspectorUpdater, public TextPropertyComponent::Listener, Value::Listener, FilenameComponentListener {
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
			DBG(obj->getName());
		
		if (selectedObj != obj) {
			updateObj();
			selectedObj = obj;
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
			selectedObj->setIdleTexture(textureFile);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Acceleration(x):") {
			float x = component->getText().getFloatValue();
			selectedObj->setXVel(x);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		if (component->getName() == "Acceleration(y):") {
			float y = component->getText().getFloatValue();
			selectedObj->setYVel(y);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		if (component->getName() == "Speed (x) Cap:") {
			float x = component->getText().getFloatValue();
			selectedObj->setXVelocityCap(x);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Speed (y) Cap:") {
			float y = component->getText().getFloatValue();
			selectedObj->setYVelocityCap(y);
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
				selectedObj->setXPosition(x);
				updateInspectorsChangeBroadcaster->sendChangeMessage();

			}

		}

		if (value.refersToSameSourceAs(yPosition)) {
			if (value.getValue().isDouble()) {
				

				float y = (float)value.getValue();
				selectedObj->setYPosition(y);
				updateInspectorsChangeBroadcaster->sendChangeMessage();

			}

		}

		if (value.refersToSameSourceAs(comboValue)) {
			
			switch ((int)comboValue.getValue()) {
			case 1:
				selectedObj->setAnimationSpeed(GameObject::SLOW);
				break;
			case 2:
				selectedObj->setAnimationSpeed(GameObject::MED);
				break;
			case 3:
				selectedObj->setAnimationSpeed(GameObject::FAST);
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
			selectedObj->setAnimationTextures(fileComponentThatHasChanged->getCurrentFile());
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (fileComponentThatHasChanged->getName() == "Choose Idle Texture") {
			selectedObj->setIdleTexture(fileComponentThatHasChanged->getCurrentFile());
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
			/*Add physics properties to object inspector
			-Linear Velocity(x)
			-Linear Velocity(y)
			*/
			//differentiate between types of objects ai, player, environment. . .
			switch (selectedObj->getObjType()) {
			case Generic:	//environment?
				break;
			case Player:	//player
				addGenericMovementProperties();
				break;
			case Enemy:	//ai, maybe differntiate between types of ai with this?
				addGenericMovementProperties();
				break;
			}
			addGenericGraphicProperties();
			//add to panel
			propertyPanel.addSection("Object Physics", objPhysicsProperties);

			//Add misc properties to panel
			propertyPanel.addSection("Misc. Properties", objBackgroundProperties);

		}

		//add Object Texture
		//add Object Audio

	}

	//base physics properties
	void addGenericMovementProperties() {


		objPhysicsX.setValue(var(selectedObj->getXVel()));
		TextPropertyComponent* objPhysicsXText = new TextPropertyComponent(objPhysicsX, "Acceleration(x):", 3, false);
		objPhysicsXText->addListener(this);
		objPhysicsProperties.add(objPhysicsXText);

		objPhysicsY.setValue(var(selectedObj->getYVel()));
		TextPropertyComponent* objPhysicsYText = new TextPropertyComponent(objPhysicsY, "Acceleration(y):", 3, false);
		objPhysicsYText->addListener(this);
		objPhysicsProperties.add(objPhysicsYText);

		objPhysicsXCap.setValue(var(selectedObj->getXVelocityCap()));
		TextPropertyComponent* objPhysicsXCapText = new TextPropertyComponent(objPhysicsXCap, "Speed (x) Cap:", 3, false);
		objPhysicsXCapText->addListener(this);
		objPhysicsProperties.add(objPhysicsXCapText);

		objPhysicsYCap.setValue(var(selectedObj->getYVelocityCap()));
		TextPropertyComponent* objPhysicsYCapText = new TextPropertyComponent(objPhysicsYCap, "Speed (y) Cap:", 3, false);
		objPhysicsYCapText->addListener(this);
		objPhysicsProperties.add(objPhysicsYCapText);

		objPhysicsFriction.setValue(var(selectedObj->getPhysicsProperties().getFriction()));
		SliderPropertyComponent* objFrictionText = new SliderPropertyComponent(objPhysicsFriction, "Friction:", 0, 1.0, 0.1);
		objPhysicsFriction.addListener(this);
		objPhysicsProperties.add(objFrictionText);

		objPhysicsRestitution.setValue(var(selectedObj->getPhysicsProperties().getRestitution()));
		SliderPropertyComponent* objRestitutionText = new SliderPropertyComponent(objPhysicsRestitution, "Bounce:", 0, 10.0, 0.1);
		objPhysicsRestitution.addListener(this);
		objPhysicsProperties.add(objRestitutionText);

		objPhysicsDensity.setValue(var(selectedObj->getPhysicsProperties().getDensity()));
		TextPropertyComponent* objDensityText = new TextPropertyComponent(objPhysicsDensity, "Density:", 3, false);
		objDensityText->addListener(this);
		objPhysicsProperties.add(objDensityText);

	}

	//base Graphical properties
	void addGenericGraphicProperties() {



		//Set objName to be the name of the selected object, and create its TextPropertyComponent
		objName.setValue(var(selectedObj->getName()));
		TextPropertyComponent* objNameText = new TextPropertyComponent(objName, "Name:", 40, false);
		objNameText->addListener(this);

		objBackgroundProperties.add(objNameText);

		//This is going to require a specification of the axis for the game
		xPosition.setValue(var((int)selectedObj->getPosition().x));
		xPosition.addListener(this);
		SliderPropertyComponent* slider = new SliderPropertyComponent(xPosition, "x-Position:", -10, 10, .25);

		//new TextPropertyComponent(xPosition, "x-Position:", 4,false);

		objBackgroundProperties.add(slider);

		//This is going to require a specification of the axis for the game
		yPosition.setValue(var((int)selectedObj->getPosition().y));
		SliderPropertyComponent* slider2 = new SliderPropertyComponent(yPosition, "y-Position:", -10, 10, .25);
		yPosition.addListener(this);
		objBackgroundProperties.add(slider2);

		//Note that ComboBoxPropertyComponent is a custom property component and not in JUCE docs
		comboValue.setValue(var((int)1));
		ComboBoxPropertyComponent* combo = new ComboBoxPropertyComponent(comboValue, "Animation Speed:");
		combo->setTextWhenNothingSelected("Choose Speed");
		combo->addItem("Fast", 3);
		combo->addItem("Normal", 2);
		combo->addItem("Slow", 1);
		comboValue.addListener(this);
		objBackgroundProperties.add(combo);

		FilenamePropertyComponent* filename = new FilenamePropertyComponent("Choose Idle Texture", selectedObj->getTexture(), false, false, false, "", "", "Select a file");
		filename->addListener(this);

		objBackgroundProperties.add(filename);

		FilenamePropertyComponent* animationDirectory = new FilenamePropertyComponent("Animation Directory", File(), false, true, false, "", "", "Select a Dir");
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
	      comboValue;

	ScopedPointer<FilenameComponent> chooseFile;

};