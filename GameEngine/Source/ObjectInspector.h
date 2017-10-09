/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"

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


		Value objPhysics, objPhysics2, objTexture, objName;

		//create additional functions in desired objects to make it easier to retrieve information.
		//add sections to physics menu
		if (selectedObj != NULL) {

			/*Add physics properties to object inspector
				-Linear Velocity(x)
				-Linear Velocity(y)
			*/
			objPhysics.setValue(var(selectedObj->getXVel()));
			objPhysicsProperties.add(new SliderPropertyComponent(objPhysics, "Linear Velocity(x):", 0.0, 10.0, 0.1));

			objPhysics2.setValue(var(selectedObj->getYVel()));
			objPhysicsProperties.add(new SliderPropertyComponent(objPhysics2, "Linear Velocity(y):", 0.0, 10.0, 0.1));

			//add to panel
			propertyPanel.addSection("Object Physics", objPhysicsProperties);

			/*Adds misc properties 
				-texture
				-object name
			*/

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
			selectedObj->setTexture(File(component->getText()));
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
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