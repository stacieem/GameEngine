#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
#include "TextureResource.h"

class TextureResourceManager {

public:

	TextureResourceManager() {

	}

	~TextureResourceManager() {
		
		
	}

	//Loads a texture from the resource manager and returns the texture reference
	OpenGLTexture* loadTexture(File texFile) {

		//if not already in the map
		std::map<File, Resource*>::iterator it;

		//Search through the map for the resource
		it = resourceMap.find(texFile);

		if (it == resourceMap.end()) {

			//Create a new texture and load in the texFile
			TextureResource* tex = new TextureResource;

			tex->loadTexture(texFile);

			resourceMap[texFile] = tex;

		}

		//Get the resource and downcast it as a TextureResource
		TextureResource* tex = (TextureResource*)resourceMap[texFile];

		return tex->getTexture();
	}

	void releaseTextures() {
		std::map<File, Resource*>::iterator it;
		for (it = resourceMap.begin(); it != resourceMap.end(); it++)
		{
			delete it->second;
		}

		resourceMap.clear();
	}

private:

	std::map<File, Resource*> resourceMap;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextureResourceManager)


};