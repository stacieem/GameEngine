#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "Level.h"
/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class CollectableObject : public GameObject {
public:
	CollectableObject(WorldPhysics & worldPhysics) : GameObject(worldPhysics)
	{
		objType = GameObjectType::Collectable;
		setName("Collectable");
		updateState(GameObject::STATIC);
		origin = getPhysicsProperties().GetPosition();
		radius = 1.5;
		isActive = true;
	}
	~CollectableObject() {}

	bool collision(PlayerObject& player) {
		bool collected = false;
		if (isActive) {
			b2Vec2 dist = (player.getPosition() - origin);
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			DBG(leng);
			if (leng < radius) {
				// go to next level
				collected = true;
				isActive = false;
				getPhysicsProperties().removeCollisionBox();
				//make this disappear so that we don't mess with it anymore
				// make it invisible

				//updateState(GameObject::DYNAMIC);
				
			}
		}
		return collected;
	}
private:
	GLfloat linearDamp;
	b2Vec2 origin;
	bool isActive;
	float32 radius;
	Vector3D<GLfloat> position;
	OwnedArray<Vector3D<GLfloat>> vertices;	 // The vertices from the origin
	ScopedPointer<GLfloat> glVertices;

	JUCE_LEAK_DETECTOR(PlayerObject)

};
