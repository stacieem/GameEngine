#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "Level.h"
/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class CollectableObject : public GameObject {
public:
	CollectableObject(WorldPhysics & worldPhysics, Model* model) : GameObject(worldPhysics, model)
	{
		objType = GameObjectType::Collectable;
		setName("Collectable");
        getPhysicsProperties().setIsStatic(true);
		radius = 1.5;
	}

	CollectableObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{
		getPhysicsProperties().setIsStatic(true);
		radius = 1.5;
	}

	/** Copy Constructor - Used to easily make a copy of an existing GameObject
	(this is directly used by the WorldNavigator when alt-dragging)
	*/
	CollectableObject(CollectableObject & objectToCopy, WorldPhysics & worldPhysics) : GameObject( objectToCopy, worldPhysics)
	{
		radius = objectToCopy.radius;

	}
	~CollectableObject() {}

	bool collision(PlayerObject& player, GameAudio& audio) {
		bool collected = false;
		radius = 1.5;
		if (getIsActive()) {
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			if (leng < radius) {
				collected = true;

				setActive(false);
				getPhysicsProperties().setActiveStatus(false);
				setRenderable(false);

				File * audioFile = getAudioFileForAction(PhysicalAction::death);

				// If audio file was not in the map, do nothing
				if (audioFile != nullptr)
				{
					audio.playAudioFile(*audioFile, false);
				}
			}
		}
		return collected;
	}

	ValueTree serializeToValueTree() {

		ValueTree collectableSerialization = GameObject::serializeToValueTree();

		return collectableSerialization;
	}

	void parseFrom(ValueTree valueTree) {

		GameObject::parseFrom(valueTree);

		radius = 1.5;
	}

private:
	float radius;
	JUCE_LEAK_DETECTOR(CollectableObject)

};
