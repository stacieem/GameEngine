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

		//Search through the map for the resource
		auto iterator = resourceMap.find(texFile);

        // If not in the map, add to the map
		if (iterator == resourceMap.end()) {

			//Create a new texture and load in the texFile
			TextureResource* tex = new TextureResource;

			tex->loadTexture(texFile);

			resourceMap[texFile] = tex;
		}

		// Get the resource and downcast it as a TextureResource
		TextureResource* tex = (TextureResource*)resourceMap[texFile];

		return tex->getTexture();
	}

	void releaseTextures() {
		for (auto iterator = resourceMap.begin(); iterator != resourceMap.end(); ++iterator)
		{
			delete iterator->second;
		}

        // MAYBE FIX LATER
        // Should we do this in destructor instead? What if we had audio
        // in these resources and not just textures? We wouldn't want releaseTextures
        // to get rid of audio too.
		resourceMap.clear();
	}

private:

	std::map<File, Resource*> resourceMap;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TextureResourceManager)


};
