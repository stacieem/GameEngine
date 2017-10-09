/*

		Lists the properties of the currently selected unit
*/
#include "CoreEngine.h"
#include "Inspector.h"

class ObjectInspector : public Component, public InspectorUpdater {
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


		Value objPhysics, objPhysics2;	//values must be seperated otherwise all sliders using the same value will update simultaneously

		//create additional functions in desired objects to make it easier to retrieve information.
		//add sections to physics menu
		if (selectedObj != NULL) {
			objPhysics.setValue(var(selectedObj->getXVel()));
			objPhysicsProperties.add(new SliderPropertyComponent(objPhysics, "Linear Velocity(x):", 0.0, 10.0, 0.1));

			objPhysics2.setValue(var(selectedObj->getYVel()));
			objPhysicsProperties.add(new SliderPropertyComponent(objPhysics2, "Linear Velocity(y):", 0.0, 10.0, 0.1));

			//adds menu
			propertyPanel.addSection("Object Physics", objPhysicsProperties);
			
		}

		//add Object Texture
		//add Object Audio
		
	}
	void resized() override
	{
		//scrollBar.setBounds(getLocalBounds());

		propertyPanel.setBounds(getLocalBounds());
		
		
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