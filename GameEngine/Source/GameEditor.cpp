
#include "GameEditor.h"


GameEditor::GameEditor() {
	setSize(1200, 800);
	addAndMakeVisible(gameEngine);
	addAndMakeVisible(ObjBrowser);
	addAndMakeVisible(ObjInspector);
	addAndMakeVisible(ObjGraph);
	addAndMakeVisible(EditorController);
	//gameEngine.setBoundsToFit(getWidth() *.2, 0, getWidth() * .5, getHeight()*.6, Justification::centredTop, true);
}

GameEditor::~GameEditor() {

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
	//gameEngine.setBoundsToFit(getWidth() *.2, 0, getWidth() * .5, getHeight()*.6, Justification::centredTop, true);
	gameEngine.setBounds(getWidth()*.3, 0, getWidth()*.5, getHeight()*.5);						//GameView panel
	ObjBrowser.setBounds(getWidth()*.8, 0, getWidth()*.2, getHeight()*.5);						//TopRight panel displaying drag/drop objects
	ObjInspector.setBounds(getWidth()*.8, getHeight()*.5, getWidth()*.2, getHeight()*.5);		//botRight panel displaying properties of selected object
	ObjGraph.setBounds(0, 0, getWidth()*.3, getHeight());										//Leftmost panel displaying all elements present in level
	EditorController.setBounds(getWidth()*.3, getHeight()*.5, getWidth()*.5, getHeight()*.5);	//centered bottom panel displaying controls
																								// etc. start/stop
}