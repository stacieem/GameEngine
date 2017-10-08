/*

	holds an array of all draggable/droppable objects
	Objects mostly
*/

#include "CoreEngine.h"
class ObjectBrowser : public Component {
public:
	ObjectBrowser() {
		addAndMakeVisible(player);
		addAndMakeVisible(enemy);
		addAndMakeVisible(genericObj);

		player.setButtonText("Player Character");
		enemy.setButtonText("Enemy Character");
		genericObj.setButtonText("Generic Character");
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
		player.setBounds(r.getX(), 0, getWidth(), BUTTON_HEIGHT);
		enemy.setBounds(r.getX(), BUTTON_HEIGHT, getWidth(), BUTTON_HEIGHT);
		genericObj.setBounds(r.getX(), BUTTON_HEIGHT * 2, getWidth(), BUTTON_HEIGHT);
		//scrollBar.setBounds(getLocalBounds());
	}
private:
	const int NUM_VISIBLE_BUTTONS = 5;
	CoreEngine* coreEngine;
	TextButton player;
	TextButton enemy;
	TextButton genericObj;
	//Slider scrollBar;
};