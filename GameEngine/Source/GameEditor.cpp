
#include "GameEditor.h"


GameEditor::GameEditor() {
	setSize(1200, 800);

	setOpaque(true);
	
	addAndMakeVisible(gameEngine);
	addAndMakeVisible(objInspector);
	addAndMakeVisible(levelInspector);
	addAndMakeVisible(objBrowser);

	objInspector.setCoreEngine(&gameEngine);
	levelInspector.setCoreEngine(&gameEngine);
	objBrowser.setCoreEngine(&gameEngine);

	// Setup GameEditor ChangeListener/Broadcaster system
	updateInspectorsChangeBroadcaster.addChangeListener(this);

	// Set Change Broadcaster for visual updates to inspectors
	objInspector.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);
	levelInspector.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);
	objBrowser.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);



	//what does this do -- this was originally my work around for an issue that arose, good now
	//while (gameEngine.getGameModel().getCurrentLevel().getNumGameObjects() < 1) {}
	
	//addAndMakeVisible(EditorController);
	//gameEngine.setBoundsToFit(getWidth() *.2, 0, getWidth() * .5, getHeight()*.6, Justification::centredTop, true);

	updateInspectors();
}

GameEditor::~GameEditor() {

}
CoreEngine& GameEditor::getCoreEngine() {
	return gameEngine;
}
void GameEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

void GameEditor::resized()
{
	// This is called when the MainContentComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
	gameEngine.setBounds(getWidth()*.3, 0, getWidth()*.5, getHeight());						//GameView panel
	objBrowser.setBounds(getWidth()*.8, 0, getWidth()*.2, getHeight()*.5);						//TopRight panel displaying drag/drop objects
	objInspector.setBounds(getWidth()*.8, getHeight()*.5, getWidth()*.2, getHeight()*.5);		//botRight panel displaying properties of selected object
	levelInspector.setBounds(0, 0, getWidth()*.3, getHeight());										//Leftmost panel displaying all elements present in level
	//EditorController.setBounds(getWidth()*.3, getHeight()*.5, getWidth()*.5, getHeight()*.5);	//centered bottom panel displaying controls
																								// etc. start/stop
}

void GameEditor::updateInspectors()
{
	levelInspector.updateInspector(gameEngine.getCurrentLevel());
	objInspector.setSelectedObj(levelInspector.getSelectedGameObject());
}

void GameEditor::changeListenerCallback(ChangeBroadcaster * source)
{
	// Update all Components that diplay GameModel information
	if (source == &updateInspectorsChangeBroadcaster)
	{
		if (gameEngine.isPaused()) {
			objBrowser.setEnabled(true);
			objInspector.setEnabled(true);
			levelInspector.setChildrenEnabled(true);
			updateInspectors();
			
		} else {
			objBrowser.setEnabled(false);
			objInspector.setEnabled(false);
			levelInspector.setChildrenEnabled(false);

		}
		
	}

}