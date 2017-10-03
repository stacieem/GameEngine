/*

		Lists the properties of the currently selected unit
*/
class ObjectInspector : public Component {
public:
	ObjectInspector() {
		addAndMakeVisible(scrollBar);
		scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

	}
	~ObjectInspector() {

	}

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		g.fillAll(Colours::coral);
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