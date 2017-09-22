#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrawableObject.h"

class RenderSwapFrame {
public:

	RenderSwapFrame() {
		drawableObjects.add(new DrawableObject());
		drawableObjects[0]->setTexture("Flower");

		drawableObjects.add(new DrawableObject());
		drawableObjects[1]->setTexture("Kenny");
	}

	~RenderSwapFrame() {

	}

	void setDrawableObjectsLength(int length) {
		drawableObjects.ensureStorageAllocated(length);
	}

	void setDrawableObjectVertices(Array<Vertex> verts, int pos) {

		drawableObjects[pos]->setVertices(verts);
	}

	OwnedArray<DrawableObject>& getDrawableObjects()
	{
		return drawableObjects;
	}

private:

	//Drawable representations of GameObjects
	OwnedArray<DrawableObject> drawableObjects;

	//Any other data that needs to be passed to the GameView (such as HUD data) will be stored here

};

