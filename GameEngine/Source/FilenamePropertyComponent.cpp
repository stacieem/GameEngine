#include "FilenamePropertyComponent.h"

FilenamePropertyComponent::FilenamePropertyComponent(const String & name, const File & currentFile, bool canEditFilename, bool isDirectory, bool isForSaving, const String & fileBrowserWildcard, const String & enforcedSuffix, const String & textWhenNothingSelected)
: PropertyComponent(name) {
	filenameComponent = new FilenameComponent(name, currentFile, canEditFilename, isDirectory, isForSaving, fileBrowserWildcard, enforcedSuffix, textWhenNothingSelected);
	addAndMakeVisible(filenameComponent);
}

FilenamePropertyComponent::~FilenamePropertyComponent()
{
}

void FilenamePropertyComponent::refresh()
{
}

void FilenamePropertyComponent::addListener(FilenameComponentListener * listener)
{
	filenameComponent->addListener(listener);
}

void FilenamePropertyComponent::removeListener(FilenameComponentListener * listener)
{
	filenameComponent->removeListener(listener);
}

File FilenamePropertyComponent::getCurrentFile() const
{
	return filenameComponent->getCurrentFile();
}
