#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ObjectInspector.h"
#include "ObjectBrowser.h"
#include "LevelInspector.h"
#include "CoreEngine.h"

class GameEditor : public Component, public ChangeListener {
public:
	//==========================================================================
	GameEditor();
	~GameEditor();
	CoreEngine& getCoreEngine();
	// JUCE GUI Callbacks ======================================================
	void paint(Graphics&) override;
	void resized() override;

	/** A JUCE ChangeListener Callback that is used to notify all visual GameEditor
		Components that the GameModel of the CoreEngine has been modified and
		viusal updates should be made.
	*/
	void changeListenerCallback(ChangeBroadcaster * source) override;

private:

	/** Updates inspector components, triggering visual updates when the GameModel
		is modified.
	*/
	void updateInspectors();

	/**	A change broadcaster that will be used to signal the GameEditor that
		the GameModel has changed and inspectors need to be updated.
	*/
	ChangeBroadcaster updateInspectorsChangeBroadcaster;

    /** Engine running the game */
	CoreEngine gameEngine;

    // Game Editing GUI Panels =================================================

    /** Inspects the current Level and allows user to switch levels */
    LevelInspector levelInspector;
    
    /** Inspects the currently selected GameObject */
    ObjectInspector objInspector;
    
    /** Library of default GameObjects for user to pick from and use in game */
	ObjectBrowser objBrowser;
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameEditor)
};
