
#include "GameEditor.h"


GameEditor::GameEditor() : levelInspector(worldNavigator) {
	
    // Default window size
    setSize(1200, 800);

	setOpaque(true);
	
    // Add components to the editor
	addAndMakeVisible(gameEngine);
	addAndMakeVisible(objInspector);
	addAndMakeVisible(levelInspector);
	addAndMakeVisible(objBrowser);
    addAndMakeVisible(worldNavigator);
    worldNavigator.broughtToFront();

    // Register inspectors with CoreEngine so they can access Controller (MVC)
    // functions to get and set GameModel data
	objInspector.setCoreEngine(&gameEngine);
	levelInspector.setCoreEngine(&gameEngine);
	objBrowser.setCoreEngine(&gameEngine);

	// Setup GameEditor ChangeListener/Broadcaster system
	updateInspectorsChangeBroadcaster.addChangeListener(this);

	// Set Change Broadcaster for visual updates to inspectors
	objInspector.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);
	levelInspector.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);
	objBrowser.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);
    worldNavigator.setChangeBroadcasterForUpdate(&updateInspectorsChangeBroadcaster);


	//what does this do -- this was originally my work around for an issue that arose, good now
	//while (gameEngine.getGameModel().getCurrentLevel().getNumGameObjects() < 1) {}
	
	//addAndMakeVisible(EditorController);
	//gameEngine.setBoundsToFit(getWidth() *.2, 0, getWidth() * .5, getHeight()*.6, Justification::centredTop, true);

    gameEngine.setWantsKeyboardFocus(true);

	updateInspectors();
    
    // Start the CoreEngine
    gameEngine.startThread();
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
    // Bounds of the GameView to be rendered
    Rectangle<int> gameViewBounds (getWidth()*0.3, 0, getWidth()*0.5, getHeight());
    
    // Size CoreEngine which holds the GameView
	gameEngine.setBounds (gameViewBounds);
    
    // Size WorldNavigator invisible overlay over GameView
    worldNavigator.setBounds (gameViewBounds);
    
    // Left Side
    levelInspector.setBounds (0, 0, getWidth()*0.3, getHeight());
    
    // Top right
	objBrowser.setBounds (getWidth()*0.8, 0, getWidth()*0.2, getHeight()*0.5);
	
    // Bottom right
    objInspector.setBounds (getWidth()*0.8, getHeight()*0.5, getWidth()*0.2, getHeight()*0.5);
}

void GameEditor::updateInspectors()
{
    // Get Game Model
    GameModel & gameModel = gameEngine.getGameModel();
    
    // Update Inspectors
	levelInspector.updateInspector (gameModel, worldNavigator.getSelectedObject());
   	objInspector.setSelectedObj (worldNavigator.getSelectedObject());
    
    // Update Navigator
    worldNavigator.setLevelToHandle (gameModel.getCurrentLevel());
    worldNavigator.setCameraToHandle (&gameModel.getCurrentLevel()->getCamera());
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
            worldNavigator.setEnabled(true);
			updateInspectors();
			
		} else {
			objBrowser.setEnabled(false);
			objInspector.setEnabled(false);
			levelInspector.setChildrenEnabled(false);
            worldNavigator.setEnabled(false);
		}
		
	}
}
