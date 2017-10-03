/*

		Displays a hierarchy of all elements present in the level(scene)
*/
class ObjectGraph : public Component {
public:
	ObjectGraph() {
		addAndMakeVisible(scrollBar);
		scrollBar.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);

	}
	~ObjectGraph() {

	}

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics& g) override {
		g.fillAll(Colours::indigo);
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