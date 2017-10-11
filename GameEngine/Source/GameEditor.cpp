
#include "GameEditor.h"


GameEditor::GameEditor() {
	
    // Default window size
    setSize(1200, 800);

	setOpaque(true);
	
    // Add components to the editor
	addAndMakeVisible(gameEngine);
	addAndMakeVisible(objInspector);
	addAndMakeVisible(levelInspector);
	addAndMakeVisible(objBrowser);

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
    
    // Update Inspectors to the starting state
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
    // Setting the bounds of the CoreEngine sets bounds of the GameView
	gameEngine.setBounds(getWidth()*0.3, 0, getWidth()*0.5, getHeight());
    
    // Left Side
    levelInspector.setBounds(0, 0, getWidth()*0.3, getHeight());
    
    // Top right
	objBrowser.setBounds(getWidth()*0.8, 0, getWidth()*0.2, getHeight()*0.5);
	
    // Bottom right
    objInspector.setBounds(getWidth()*0.8, getHeight()*0.5, getWidth()*0.2, getHeight()*0.5);
}

void GameEditor::updateInspectors()
{
    // Get Game Model
    GameModel & gameModel = gameEngine.getGameModel();
    
    // Update Inspectors
	levelInspector.updateInspector(gameModel);
    
    // WE WANT THIS TO BE THE CURRENTLY SELECTED OBJECT SO FIX LATER!!!!
	objInspector.updateObj(gameModel.getCurrentLevel()->getGameObjects().getFirst());
}

void GameEditor::changeListenerCallback(ChangeBroadcaster * source)
{
	// Update all Components that diplay GameModel information
	if (source == &updateInspectorsChangeBroadcaster)
	{
		updateInspectors();
	}
}
