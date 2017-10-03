#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ObjectInspector.h"
#include "ObjectBrowser.h"
#include "ObjectGraph.h"
#include "CoreEngine.h"
#include "EditorControl.h"


class GameEditor : public Component {
public:
	//==========================================================================
	GameEditor();
	~GameEditor();

	// JUCE GUI Callbacks ======================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	CoreEngine gameEngine;
	EditorControl EditorController;

	ObjectInspector ObjInspector;
	ObjectBrowser ObjBrowser;
	ObjectGraph ObjGraph;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameEditor)
};