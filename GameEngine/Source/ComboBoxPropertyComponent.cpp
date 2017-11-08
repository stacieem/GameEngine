#include "ComboBoxPropertyComponent.h"

ComboBoxPropertyComponent::ComboBoxPropertyComponent(const Value& valueToControl, const String& propertyName) : PropertyComponent(propertyName)
{
	addAndMakeVisible(comboBox);
	comboBox.getSelectedIdAsValue().referTo(valueToControl);
	comboBox.addListener(this);
}

ComboBoxPropertyComponent::~ComboBoxPropertyComponent()
{
}

void ComboBoxPropertyComponent::setValue(const int /*newValue*/)
{
}

int ComboBoxPropertyComponent::getValue() const
{
	return comboBox.getSelectedId();
}

void ComboBoxPropertyComponent::addItem(String itemText, int itemID)
{
	comboBox.addItem(itemText, itemID);
}

void ComboBoxPropertyComponent::setTextWhenNothingSelected(const String & newMessage)
{
	comboBox.setTextWhenNothingSelected(newMessage);
}

String ComboBoxPropertyComponent::getTextWhenNothingSelected() const
{
	return comboBox.getTextWhenNothingSelected();
}

void ComboBoxPropertyComponent::setEditableText(bool isEditable)
{
	comboBox.setEditableText(isEditable);
}

bool ComboBoxPropertyComponent::isTextEditable() const noexcept
{
	return comboBox.isTextEditable();
}

void ComboBoxPropertyComponent::setItemEnabled(int itemId, bool shouldBeEnabled)
{
	comboBox.setItemEnabled(itemId, shouldBeEnabled);
}

bool ComboBoxPropertyComponent::isItemEnabled(int itemId) const noexcept
{
	return comboBox.isItemEnabled(itemId);
}

void ComboBoxPropertyComponent::changeItemText(int itemId, const String & newText)
{
	comboBox.changeItemText(itemId, newText);
}

void ComboBoxPropertyComponent::comboBoxChanged(ComboBox*) 
{
	if (getValue() != comboBox.getSelectedId())
		setValue(comboBox.getSelectedId());
	
}

void ComboBoxPropertyComponent::setSelectedId(int newItemId, NotificationType notification)
{
	comboBox.setSelectedId(newItemId, notification);
}

void ComboBoxPropertyComponent::refresh()
{
	comboBox.setSelectedId(getValue(), dontSendNotification);
}