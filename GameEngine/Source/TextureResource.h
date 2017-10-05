#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Resource.h"

class TextureResource : public Resource {

public:

	TextureResource() : Resource() {
		texture = nullptr;
	}

	~TextureResource() {
		if (texture != nullptr) {
			DBG("delete texture");
			delete texture;
		}
	}

	void loadTexture(File texFile) {


		//Create image from the texture file
		Image textureImage = ImageFileFormat::loadFrom(texFile);
			
		//From JUCE
		// Image must have height and width equal to a power of 2 pixels to be more efficient
		// when used with older GPU architectures
		if (!(isPowerOfTwo(textureImage.getWidth()) && isPowerOfTwo(textureImage.getHeight()))) {
			textureImage = textureImage.rescaled(jmin(1024, nextPowerOfTwo(textureImage.getWidth())),
				jmin(1024, nextPowerOfTwo(textureImage.getHeight())));
		}

		// Use that image as a 2-D texture for the object that will be painted
		texture = new OpenGLTexture();

		//Load the image into the texture
		texture->loadImage(textureImage);

	}

	OpenGLTexture* getTexture() {
		return texture;
	}

private:

	OpenGLTexture* texture;

	JUCE_LEAK_DETECTOR(TextureResource)

};