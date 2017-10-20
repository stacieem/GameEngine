#pragma once
//==============================================================================
/**
A PropertyComponent that shows its value as a FilenameComponent.

@see PropertyComponent, FilenameComponent
*/
#include "../JuceLibraryCode/JuceHeader.h"


class FilenamePropertyComponent : public PropertyComponent {
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
	FilenamePropertyComponent(const String &name, const File &currentFile, bool canEditFilename, bool isDirectory, bool isForSaving, const String &fileBrowserWildcard, const String &enforcedSuffix, const String &textWhenNothingSelected);


	/** Destructor. */
	~FilenamePropertyComponent();


	//==============================================================================
	/** @internal */
	void refresh();
	/** @internal */
	
	/** Adds a listener that will be called when the selected file is changed. */
	void addListener(FilenameComponentListener* listener);

	/** Removes a previously-registered listener. */
	void removeListener(FilenameComponentListener* listener);

	/** Returns the currently displayed filename. */
	File getCurrentFile() const;


protected:
	/** The ComboBox component being used in this component.
	Your subclass has access to this in case it needs to customise it in some way.
	*/
	ScopedPointer<FilenameComponent> filenameComponent;

private:
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilenamePropertyComponent)
};