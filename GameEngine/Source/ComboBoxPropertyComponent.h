#pragma once
//==============================================================================
/**
A PropertyComponent that shows its value as a ComboBox.

@see PropertyComponent, ComboBox
*/
#include "../JuceLibraryCode/JuceHeader.h"


class ComboBoxPropertyComponent : public PropertyComponent, public ComboBox::Listener {
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
	ComboBoxPropertyComponent(const Value& valueToControl,
		const String& propertyName);

	/** Destructor. */
	~ComboBoxPropertyComponent();

	//==============================================================================
	/** Called when the user changes the ComboBox value.

	Your subclass must use this method to update whatever item this property
	represents.
	*/
	virtual void setValue(int newValue);

	/** Returns the value that the ComboBox should show. */
	virtual int getValue() const;

	void addItem(String itemText, int itemID);

	/** Sets a message to display when there is no item currently selected.
	@see getTextWhenNothingSelected
	*/
	void setTextWhenNothingSelected(const String& newMessage);

	/** Returns the text that is shown when no item is selected.
	@see setTextWhenNothingSelected
	*/
	String getTextWhenNothingSelected() const;

	/** Sets whether the text in the combo-box is editable.

	The default state for a new ComboBox is non-editable, and can only be changed
	by choosing from the drop-down list.
	*/
	void setEditableText(bool isEditable);

	/** Returns true if the text is directly editable.
	@see setEditableText
	*/
	bool isTextEditable() const noexcept;

	/** This allows items in the drop-down list to be selectively disabled.

	When you add an item, it's enabled by default, but you can call this
	method to change its status.

	If you disable an item which is already selected, this won't change the
	current selection - it just stops the user choosing that item from the list.
	*/
	void setItemEnabled(int itemId, bool shouldBeEnabled);

	/** Returns true if the given item is enabled. */
	bool isItemEnabled(int itemId) const noexcept;

	/** Changes the text for an existing item.
	*/
	void changeItemText(int itemId, const String& newText);

	/** Sets one of the items to be the current selection.

	This will set the ComboBox's text to that of the item that matches
	this ID.

	@param newItemId        the new item to select
	@param notification     determines the type of change notification that will
	be sent to listeners if the value changes
	@see getSelectedId, setSelectedItemIndex, setText
	*/
	void setSelectedId(int newItemId,
		NotificationType notification = sendNotificationAsync);

	//==============================================================================
	/** @internal */
	void refresh();
	/** @internal */
	void comboBoxChanged(ComboBox* comboBox);


protected:
	/** The ComboBox component being used in this component.
	Your subclass has access to this in case it needs to customise it in some way.
	*/
	ComboBox comboBox;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ComboBoxPropertyComponent)
};