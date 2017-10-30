#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Speed.h"

class AnimationProperties {
public:

	AnimationProperties() {

		isAnimating = false;
		leftAnimation = false;
		animationTotalTime = 450;
	}

	AnimationProperties(const AnimationProperties& obj) {
		animationTextureFiles = obj.animationTextureFiles;
		idleTexture = obj.idleTexture;
	    animationDirectory = obj.animationDirectory;

		canimate = obj.canimate;
		isAnimating = obj.isAnimating;
		leftAnimation = obj.leftAnimation;

		animationStartTime = obj.animationStartTime;
		animationCurrentTime = obj.animationCurrentTime;
		animationTotalTime = obj.animationTotalTime;


		animationSpeed;

	}

	AnimationProperties& operator=(const AnimationProperties& obj) {
		animationTextureFiles = obj.animationTextureFiles;
		idleTexture = obj.idleTexture;
		animationDirectory = obj.animationDirectory;

		canimate = obj.canimate;
		isAnimating = obj.isAnimating;
		leftAnimation = obj.leftAnimation;

		animationStartTime = obj.animationStartTime;
		animationCurrentTime = obj.animationCurrentTime;
		animationTotalTime = obj.animationTotalTime;


		animationSpeed;

		return *this;
	}

	~AnimationProperties() {

	}

	bool getIsAnimating() {
		return isAnimating;
	}

	void setIsAnimating(bool isAnimating) {
		this->isAnimating = isAnimating;

	}

	void setLeftAnimation(bool isLeft) {
		/*if (isLeft != leftAnimation) {
			reverseTextureCoords();

		}*/

		this->leftAnimation = isLeft;

	}

	bool isLeftAnimation() {
		return leftAnimation;
	}

	bool getCanimate() {
		return canimate;
	}

	void setCanimate(bool canimate) {
		this->canimate = canimate;
	}

	void setAnimationStartTime(int64 animationStartTime) {
		this->animationStartTime = animationStartTime;
	}

	void updateAnimationCurrentTime(int64 animationCurrentTime) {
		this->animationCurrentTime = animationCurrentTime;
	}

	void setAnimationSpeed(Speed animationSpeed) {
		this->animationSpeed = animationSpeed;
	}

	Speed getAnimationSpeed() {
		return animationSpeed;
	}

	File getIdleTexture() {
		return idleTexture;
	}

	File getAnimationTextureDirectory() {
		return animationDirectory;
	}

	File getTexture() {

		if (!canimate) {
			return idleTexture;
		}

		if (!isAnimating) {

			return idleTexture;
		}

		if (animationTextureFiles.size() == 0) {
			return File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/default.png");
		}

		int computedAnimSpeed = animationTotalTime;

		switch (animationSpeed) {
		case FAST:
			computedAnimSpeed /= 2;
			break;

		case SLOW:
			computedAnimSpeed *= 2;
			break;
		}

		int size = animationTextureFiles.size();

		double currentTime = (((animationCurrentTime - animationStartTime) % computedAnimSpeed)) / 1000.0;

		int index = currentTime / ((computedAnimSpeed / 1000.0) / (double)size);


		return animationTextureFiles[index];
	}

	void setAnimationTextures(File directory) {
		animationDirectory = directory;

		animationTextureFiles.clear();

		DirectoryIterator iter(animationDirectory, false, "*.jpg;*.JPG;*.jpeg;*.JPEG;*.PNG;*.png");
		while (iter.next())
		{
			File theFileItFound(iter.getFile());
			animationTextureFiles.add(theFileItFound);
		}
	}

	void addAnimationTexture(File tex) {
		animationTextureFiles.add(tex);
	}

	void setIdleTexture(File tex) {
		idleTexture = tex;

	}

	File getTextureAt(int index) {
		if (index > animationTextureFiles.size()) {
			return File(File::getCurrentWorkingDirectory().getFullPathName() + "/textures/default.png");
		}
		return animationTextureFiles[index];

	}

	int getNumTextures() {

		return animationTextureFiles.size();


	}

	/*void reverseTextureCoords() {

		std::swap(vertices[0]->texCoord, vertices[3]->texCoord);
		std::swap(vertices[1]->texCoord, vertices[2]->texCoord);

	}*/

private:

	Array<File> animationTextureFiles;
	File idleTexture;
	File animationDirectory;

	bool canimate;
	bool isAnimating;
	bool leftAnimation;

	int64 animationStartTime;
	int64 animationCurrentTime;
	int64 animationTotalTime;


	Speed animationSpeed;


};