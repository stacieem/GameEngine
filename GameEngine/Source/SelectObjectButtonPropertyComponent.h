#pragma once
//==============================================================================
/**
A PropertyComponent that shows its value as a FilenameComponent.

@see PropertyComponent, FilenameComponent
*/
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"


class SelectObjectButtonPropertyComponent : public PropertyComponent, private Button::Listener {
protected:
	//==============================================================================
	/** Creates the property component.

	If you need to customise the ComboBox in other ways, your constructor can
	access the ComboBox member variable and change it directly.
	*/
	//ComboBoxPropertyComponent(const String& propertyName);

public:
	//==============================================================================
	/** Creates the property component.

	If you need to customise the ComboBox in other ways, your constructor can
	access the ComboBox member variable and change it directly.

	Note that if you call this constructor then you must use the Value to interact with
	the value, and you can't override the class with your own setValue or getValue methods.
	If you want to use those methods, call the other constructor instead.
	*/
	SelectObjectButtonPropertyComponent(int index, const Value& valueToControl, const String &propertyName, bool triggerOnMouseDown);


	/** Destructor. */
	~SelectObjectButtonPropertyComponent();


	//==============================================================================
	/** @internal */
	void refresh();

	void buttonStateChanged(Button *);
	void buttonClicked(Button *);


protected:
	TextButton button;
	Value value;
	int index;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectObjectButtonPropertyComponent)
};