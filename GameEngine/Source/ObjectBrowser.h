/*

	holds an array of all draggable/droppable objects
	Objects mostly
*/

#include "CoreEngine.h"
class ObjectBrowser : public Component, public Button::Listener, public InspectorUpdater {
public:
	ObjectBrowser() {
		addAndMakeVisible(enemy);
		addAndMakeVisible(block);

		enemy.setButtonText("Enemy Character");
		block.setButtonText("Block");
		block.addListener(this);
		//addAndMakeVisible(scrollBar);
		//scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

	}
	~ObjectBrowser() {

	}
	void setCoreEngine(CoreEngine* engine){
		coreEngine = engine;
	}
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {		
			//g.fillAll(Colours::aquamarine);
		 //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	}

	void resized() override
	{
		juce::Rectangle<int> r = getLocalBounds();
		int BUTTON_HEIGHT = r.getHeight() / NUM_VISIBLE_BUTTONS;
		
		enemy.setBounds(r.getX(), 0, getWidth(), BUTTON_HEIGHT);
		block.setBounds(r.getX(), BUTTON_HEIGHT, getWidth(), BUTTON_HEIGHT);
		//scrollBar.setBounds(getLocalBounds());
	}

	void buttonClicked(Button* button)
	{
		if (button == &block)
		{
			coreEngine->addBlock();
			updateInspectorsChangeBroadcaster->sendChangeMessage();
		}
	}

private:
	const int NUM_VISIBLE_BUTTONS = 5;
	CoreEngine* coreEngine;
	TextButton enemy;
	TextButton block;
	//Slider scrollBar;
};