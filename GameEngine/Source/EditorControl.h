
/*

	Able to start/stop the game world and select various levels
*/
class EditorControl : public Component {
public:
	EditorControl() {
		addAndMakeVisible(scrollBar);
		scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

	}
	~EditorControl() {

	}

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		g.fillAll(Colours::peru);
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