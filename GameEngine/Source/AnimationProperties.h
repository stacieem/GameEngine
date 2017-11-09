#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Speed.h"

class AnimationProperties {
public:

	AnimationProperties() {

		isAnimating = false;
		leftAnimation = false;
		animationTotalTime = 450;
		animationSpeed = MED;
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

	ValueTree serializeToValueTree() {

		//Create the root ValueTree to serialize the animation properties
		ValueTree animationPropertiesSerialization = ValueTree("AnimationProperties");

		//Serialize canimate
		ValueTree canimateValueTree = ValueTree("Canimate");
		canimateValueTree.setProperty(Identifier("value"), var(canimate), nullptr);
		animationPropertiesSerialization.addChild(canimateValueTree, -1, nullptr);

		//Serialize isAnimating
		ValueTree isAnimatingValueTree = ValueTree("IsAnimating");
		isAnimatingValueTree.setProperty(Identifier("value"), var(isAnimating), nullptr);
		animationPropertiesSerialization.addChild(isAnimatingValueTree, -1, nullptr);

		//Serialize leftAnimation
		ValueTree leftAnimationValueTree = ValueTree("LeftAnimation");
		leftAnimationValueTree.setProperty(Identifier("value"), var(leftAnimation), nullptr);
		animationPropertiesSerialization.addChild(leftAnimationValueTree, -1, nullptr);

		//Serialize animationTotalTime
		ValueTree animationTotalTimeValueTree = ValueTree("AnimationTotalTime");
		animationTotalTimeValueTree.setProperty(Identifier("value"), var(animationTotalTime), nullptr);
		animationPropertiesSerialization.addChild(animationTotalTimeValueTree, -1, nullptr);


		int animationSpeedInt;

		switch (animationSpeed) {
		case SLOW:
			animationSpeedInt = 0;
			break;
		case MED:
			animationSpeedInt = 1;
			break;
		case FAST:
			animationSpeedInt = 2;
			break;
		}

		ValueTree animationSpeedValueTree = ValueTree("AnimationSpeed");
		animationSpeedValueTree.setProperty(Identifier("value"), var(animationSpeedInt), nullptr);
		animationPropertiesSerialization.addChild(animationSpeedValueTree, -1, nullptr);

		ValueTree idleTextureValueTree = ValueTree("IdleTexture");
		idleTextureValueTree.setProperty(Identifier("value"), var(idleTexture.getRelativePathFrom(File::getCurrentWorkingDirectory())), nullptr);
		animationPropertiesSerialization.addChild(idleTextureValueTree, -1, nullptr);

		ValueTree animationDirectoryValueTree = ValueTree("AnimationDirectory");
		animationDirectoryValueTree.setProperty(Identifier("value"), var(animationDirectory.getRelativePathFrom(File::getCurrentWorkingDirectory())), nullptr);
		animationPropertiesSerialization.addChild(animationDirectoryValueTree, -1, nullptr);

		return animationPropertiesSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		ValueTree canimateTree = valueTree.getChildWithName(Identifier("Canimate"));

		canimate = canimateTree.getProperty(Identifier("value"));

		ValueTree isAnimatingValueTree = valueTree.getChildWithName(Identifier("IsAnimating"));

		isAnimating = isAnimatingValueTree.getProperty(Identifier("value"));

		ValueTree leftAnimationTree = valueTree.getChildWithName(Identifier("LeftAnimation"));

		leftAnimation = leftAnimationTree.getProperty(Identifier("value"));

		ValueTree animationTimeTree = valueTree.getChildWithName(Identifier("AnimationTotalTime"));

		animationTotalTime = animationTimeTree.getProperty(Identifier("value"));

		DBG(animationTotalTime);

		ValueTree speedTree = valueTree.getChildWithName(Identifier("AnimationSpeed"));

		int animationSpeedInt = speedTree.getProperty(Identifier("value"));

		switch (animationSpeedInt) {
		case 0:
			animationSpeed = SLOW;
			break;
		case 1:
			animationSpeed = MED;
			break;
		case 2:
			animationSpeed = FAST;
			break;
		}

		ValueTree animationDirectoryTree = valueTree.getChildWithName(Identifier("AnimationDirectory"));

		animationDirectory = File(File::getCurrentWorkingDirectory().getFullPathName() + "/" + animationDirectoryTree.getProperty(Identifier("value")).toString());

		ValueTree idleTextureTree = valueTree.getChildWithName(Identifier("IdleTexture"));

		idleTexture = File(File::getCurrentWorkingDirectory().getFullPathName() + "/" + idleTextureTree.getProperty(Identifier("value")).toString());

		setAnimationTextures(animationDirectory);
	}

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
