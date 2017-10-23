/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"
#include "GameObjectType.h"
#include <string>
class ObjectInspector : public Component, public InspectorUpdater, public TextPropertyComponent::Listener  {
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
		selectedObj = obj;
	}
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		//g.fillAll(Colours::coral);
		//g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	}
	void updateObj(GameObject* obj) {
		selectedObj = obj;

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
	void resized() override
	{
		//scrollBar.setBounds(getLocalBounds());

		propertyPanel.setBounds(getLocalBounds());
	}
	
	
	void textPropertyComponentChanged(TextPropertyComponent * component) override{

		//Really bad hacky solution since these are generated on the fly right now, they don't exist as member variables
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
		if (component->getName() == "x-Position:") {

			float x = component->getText().getFloatValue()/100;
			selectedObj->setXPosition(x);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "y-Position:") {
			float y = component->getText().getFloatValue() / 100;
			selectedObj->setYPosition(y);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Linear Velocity(x):") {
			float x = component->getText().getFloatValue();
			selectedObj->setXVel(x);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		if (component->getName() == "Linear Velocity(y):") {
			float y = component->getText().getFloatValue();
			selectedObj->setYVel(y);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		if (component->getName() == "Linear Velocity(x) Cap:") {
			float x = component->getText().getFloatValue();
			selectedObj->setXVelocityCap(x);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Linear Velocity(y) Cap:") {
			float y = component->getText().getFloatValue();
			selectedObj->setYVelocityCap(y);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Friction:") {
			float fric = component->getText().getFloatValue();
			selectedObj->getPhysicsProperties().setFriction(fric);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

		if (component->getName() == "Restitution:") {
			float rest = component->getText().getFloatValue();
			selectedObj->getPhysicsProperties().setRestitution(rest);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
		if (component->getName() == "Density:") {
			float dens = component->getText().getFloatValue()/100;
			selectedObj->getPhysicsProperties().setDensity(dens);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}

	}

	//base physics properties
	void addGenericMovementProperties() {
		Value objPhysicsX, objPhysicsY, objPhysicsXCap, objPhysicsYCap,
			  objPhysicsFriction, objPhysicsRestitution, objPhysicsDensity;

		objPhysicsX.setValue(var(selectedObj->getXVel()));
		TextPropertyComponent* objPhysicsXText = new TextPropertyComponent(objPhysicsX, "Linear Velocity(x):", 3, false);
		objPhysicsXText->addListener(this);
		objPhysicsProperties.add(objPhysicsXText);

		objPhysicsY.setValue(var(selectedObj->getYVel()));
		TextPropertyComponent* objPhysicsYText = new TextPropertyComponent(objPhysicsY, "Linear Velocity(y):", 3, false);
		objPhysicsYText->addListener(this);
		objPhysicsProperties.add(objPhysicsYText);

		objPhysicsXCap.setValue(var(selectedObj->getXVelocityCap()));
		TextPropertyComponent* objPhysicsXCapText = new TextPropertyComponent(objPhysicsXCap, "Linear Velocity(x) Cap:", 3, false);
		objPhysicsXCapText->addListener(this);
		objPhysicsProperties.add(objPhysicsXCapText);

		objPhysicsYCap.setValue(var(selectedObj->getYVelocityCap()));
		TextPropertyComponent* objPhysicsYCapText = new TextPropertyComponent(objPhysicsYCap, "Linear Velocity(y) Cap:", 3, false);
		objPhysicsYCapText->addListener(this);
		objPhysicsProperties.add(objPhysicsYCapText);

		objPhysicsFriction.setValue(var(selectedObj->getPhysicsProperties().getFriction()));
		TextPropertyComponent* objFrictionText = new TextPropertyComponent(objPhysicsFriction, "Friction:", 3, false);
		objFrictionText->addListener(this);
		objPhysicsProperties.add(objFrictionText);

		objPhysicsRestitution.setValue(var(selectedObj->getPhysicsProperties().getRestitution()));
		TextPropertyComponent* objRestitutionText = new TextPropertyComponent(objPhysicsRestitution, "Restitution:", 3, false);
		objRestitutionText->addListener(this);
		objPhysicsProperties.add(objRestitutionText);

		objPhysicsDensity.setValue(var(selectedObj->getPhysicsProperties().getDensity()));
		TextPropertyComponent* objDensityText = new TextPropertyComponent(objPhysicsDensity, "Density:", 3, false);
		objDensityText->addListener(this);
		objPhysicsProperties.add(objDensityText);

	}

	//base Graphical properties
	void addGenericGraphicProperties() {
		Value objTexture, objName, xPosition, yPosition;
		//Set objTexture to be the file path of the selected object's texture, and create its TextPropertyComponent
		//Note that this should change to be a file picker
		objTexture.setValue(var(selectedObj->getTexture().getFullPathName()));
		TextPropertyComponent* objTextureText = new TextPropertyComponent(objTexture, "Texture:", 400, false);
		objTextureText->addListener(this);

		objBackgroundProperties.add(objTextureText);

		//Set objName to be the name of the selected object, and create its TextPropertyComponent
		objName.setValue(var(selectedObj->getName()));
		TextPropertyComponent* objNameText = new TextPropertyComponent(objName, "Name:", 40, false);
		objNameText->addListener(this);

		objBackgroundProperties.add(objNameText);

		//This is going to require a specification of the axis for the game
		xPosition.setValue(var((int)selectedObj->getPosition().x* 10));
		TextPropertyComponent* slider = new TextPropertyComponent(xPosition, "x-Position:", 4,false);
		slider->addListener(this);
		objBackgroundProperties.add(slider);

		//This is going to require a specification of the axis for the game
		yPosition.setValue(var((int)selectedObj->getPosition().y));
		TextPropertyComponent* slider2 = new TextPropertyComponent(yPosition, "y-Position:", 4, false);
		slider2->addListener(this);
		objBackgroundProperties.add(slider2);
	}
private:
	CoreEngine* coreEngine;
	GameObject* selectedObj;
	//Slider scrollBar;
	//selected object properties
	PropertyPanel propertyPanel;
	Array<PropertyComponent *> objPhysicsProperties;
	Array<PropertyComponent *> objAudioProperties;
	Array<PropertyComponent *> objBackgroundProperties;
};