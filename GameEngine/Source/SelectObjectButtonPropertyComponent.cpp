#include "SelectObjectButtonPropertyComponent.h"

SelectObjectButtonPropertyComponent::SelectObjectButtonPropertyComponent(int index, const Value& valueToControl, const String & propertyName, bool triggerOnMouseDown) : PropertyComponent(propertyName)
{
	jassert(index > -1);
	this->index = index;
	addAndMakeVisible(button);
	button.setButtonText("Select Object");
	button.setTriggeredOnMouseDown(triggerOnMouseDown);
	button.addListener(this);
	value.referTo(valueToControl);
}

SelectObjectButtonPropertyComponent::~SelectObjectButtonPropertyComponent()
{
}

void SelectObjectButtonPropertyComponent::refresh()
{
	//button.setButtonText(getButtonText());
}

void SelectObjectButtonPropertyComponent::buttonStateChanged(Button *)
{
}

void SelectObjectButtonPropertyComponent::buttonClicked(Button *)
{
	value.setValue(var((int)index));
}


