/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"
#include "GameObjectType.h"

class ObjectInspector : public Component, public InspectorUpdater, public TextPropertyComponent::Listener {
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
	
	
	void textPropertyComponentChanged(TextPropertyComponent * component) {

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
			selectedObj->setTexture(textureFile);
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}
	void sliderValueChanged(TextPropertyComponent * component) {
		if (component->getName() == "x-Position:") {
			
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}

	//base physics properties
	void addGenericMovementProperties() {
		Value objPhysicsX, objPhysics2Y, objPhysicsXCap, objPhysicsYCap,
			  objPhysicsFriction, objPhysicsRestitution, objPhysicsDensity;

		objPhysicsX.setValue(var(selectedObj->getXVel()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsX, "Linear Velocity(x):", 0.0, 10.0, 0.1));

		objPhysics2Y.setValue(var(selectedObj->getYVel()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysics2Y, "Linear Velocity(y):", 0.0, 10.0, 0.1));


		objPhysicsXCap.setValue(var(selectedObj->getXVelocityCap()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsXCap, "Linear Velocity(x) Cap:", 0.0, 10.0, 0.1));

		objPhysicsYCap.setValue(var(selectedObj->getYVelocityCap()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsYCap, "Linear Velocity(y) Cap:", 0.0, 10.0, 0.1));


		objPhysicsFriction.setValue(var(selectedObj->getPhysicsProperties().getFriction()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsFriction, "Friction:", 0.0, 1.0, 0.01));


		objPhysicsRestitution.setValue(var(selectedObj->getPhysicsProperties().getRestitution()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsRestitution, "Restitution:", 0.0, 1.0, 0.01));

		objPhysicsDensity.setValue(var(selectedObj->getPhysicsProperties().getDensity()));
		objPhysicsProperties.add(new SliderPropertyComponent(objPhysicsDensity, "Density:", 0.0, 10.0, 0.1));

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
		xPosition.setValue(var(selectedObj->getPosition().x * 100));
		objBackgroundProperties.add(new SliderPropertyComponent(xPosition, "x-Position:", -100, 100, 1));

		yPosition.setValue(var(selectedObj->getPosition().y*100));
		objBackgroundProperties.add(new SliderPropertyComponent(yPosition, "y-Position:", 0.0, 100.0, 1));
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