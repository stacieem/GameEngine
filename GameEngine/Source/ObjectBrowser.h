/*

	holds an array of all draggable/droppable objects
	Objects mostly
*/
class ObjectBrowser : public Component {
public:
	ObjectBrowser() {
		addAndMakeVisible(scrollBar);
		scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
	
	}
	~ObjectBrowser() {

	}

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {		
			g.fillAll(Colours::aquamarine);
		 //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	}

	void resized() override
	{
		scrollBar.setBounds(getLocalBounds());
	}
private:
	Slider scrollBar;
	// Game Data Model
	//OwnedArray<GameObject> gameObjects;
};