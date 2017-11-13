#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "GameObject.h"
#include "PlayerObject.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
*/
class EnemyObject : public GameObject {
public:
	EnemyObject(WorldPhysics & worldPhysics, Model* model) : GameObject(worldPhysics, model)
	{
		objType = GameObjectType::Enemy;

		aiState = NONE;

		setMoveSpeed(Speed::SLOW);
		setJumpSpeed(Speed::SLOW);
		detection_radius = 7;
		setName("Enemy");
		direction = 1;
		timeToSwap = 200;	//value that deltaTime will sum to for us to determine we need to switch directions
		timeElapsed = 0;

		radius = 1.5;
        getPhysicsProperties().setIsStatic(false);
	}

	/** Copy Constructor - Used to easily make a copy of an existing GameObject
	(this is directly used by the WorldNavigator when alt-dragging)
	*/
	EnemyObject(EnemyObject & objectToCopy, WorldPhysics & worldPhysics) : GameObject(objectToCopy, worldPhysics)
	{
		aiState = objectToCopy.aiState;
		detection_radius = 7;
		direction = 1;
		timeToSwap = 200;	//value that deltaTime will sum to for us to determine we need to switch directions
		timeElapsed = 0;

		radius = 1.5;
	}

	EnemyObject(WorldPhysics & worldPhysics, Model* model, ValueTree valueTree) : GameObject(worldPhysics, model, valueTree)
	{

		detection_radius = 7;
		setName("Enemy");
		direction = 1;
		timeToSwap = 200;	//value that deltaTime will sum to for us to determine we need to switch directions
		timeElapsed = 0;
		radius = 1.5;

	}

	~EnemyObject() {}

	enum AIType {
		NONE,
		GROUNDPATROL,
		JUMPPATROL,
		SCAREDAF,
		CHASE
	};
	void changeAI(AIType type) {
		aiState = type;
	}

	AIType getAIState() {
		return aiState;
	}
	bool collision(PlayerObject& player, GameAudio &audio, int points) {
		bool damage = false;
			b2Vec2 dist = (player.getPosition() - getPhysicsProperties().GetPosition());
			float leng = sqrt(dist.x * dist.x + dist.y*dist.y);
			if (leng < radius) {	//check if collided
				if (player.getPosition().y > this->getPhysicsProperties().GetPosition().y+.3) {	//if player kills enemy
					setActive(false);
					getPhysicsProperties().setActiveStatus(false);
					setRenderable(false);

					File * audioFile = getAudioFileForAction(PhysicalAction::death);

					// If audio file was not in the map, do nothing
					if (audioFile != nullptr)
					{
						audio.playAudioFile(*audioFile, false);
					}
					player.addCurrScore(points);
				}
				else
				{
					damage = true;
				}
			}

		return damage;
	}
	void decision(PlayerObject& player, double elapsed) {
		
		switch (aiState) {
		case GROUNDPATROL:
			if (timeElapsed >= timeToSwap) {
				timeElapsed = 0;
				direction *= -1;
				b2Vec2 store = getPhysicsProperties().getLinearVel();
				getPhysicsProperties().setLinearVelocity(0, store.y);
			}
			else
			{
				timeElapsed += 1;
			}
			moveLateral();
			break;
		case JUMPPATROL:
			if (timeElapsed >= timeToSwap) {
				timeElapsed = 0;
				direction *= -1;
				b2Vec2 store = getPhysicsProperties().getLinearVel();
				getPhysicsProperties().setLinearVelocity(0, store.y);
			}
			else
			{
				timeElapsed += 1;
			}
			jumpLateral();
			break;
		case CHASE:
			if ((b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y) - 
					b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y)).Length() < detection_radius)	//detected
			{
				b2Vec2 myPos = b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y);
				b2Vec2 theirPos = b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y);
				if (myPos.x < theirPos.x) {
					moveRight();
				}
				else if (myPos.x > theirPos.x)
				{
					moveLeft();
				}
			}
			break;
		case SCAREDAF:
			if ((b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y) - 
					b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y)).Length() < detection_radius)	//detected
			{
				b2Vec2 myPos = b2Vec2(getRenderableObject().position.x, getRenderableObject().position.y);
				b2Vec2 theirPos = b2Vec2(player.getRenderableObject().position.x, player.getRenderableObject().position.y);
				if (myPos.x > theirPos.x) {
					moveRight();
				}
				else if (myPos.x < theirPos.x)
				{
					moveLeft();
				}
			}
			break;
		}
	}
	void moveUp()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y = -getJumpSpeedVelocity() / 2;
		if (store.y > cappedJumpSpeed) {
			store.y = cappedJumpSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveDown()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.y = getJumpSpeedVelocity() / 2;
		if (store.y < -cappedJumpSpeed) {
			store.y = -cappedJumpSpeed;
		}

		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveLeft()
	{

		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = -getRunSpeedVelocity();
		if (store.x < -cappedMoveSpeed) {
			store.x = -cappedMoveSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
		//getPhysicsProperties().setLinearDamping(linearDamp);
	}
	void moveRight()
	{
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getRunSpeedVelocity();
		if (store.x > cappedMoveSpeed) {
			store.x = cappedMoveSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void moveLateral() {
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getRunSpeedVelocity() * direction;
		if (store.x > cappedMoveSpeed) {
			store.x = cappedMoveSpeed;
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}
	void jumpLateral() {
		b2Vec2 store = getPhysicsProperties().getLinearVel();
		store.x = getRunSpeedVelocity() * direction;
		if (store.x > cappedMoveSpeed) {
			store.x = cappedMoveSpeed;
		}
		if (store.y == 0) {
			store.y = getJumpSpeedVelocity();
		}
		getPhysicsProperties().setLinearVelocity(store.x, store.y);
	}

	ValueTree serializeToValueTree() {

		ValueTree enemySerialization = GameObject::serializeToValueTree();

		ValueTree aiStateValueTree("AIState");

		int aiStateInt;

		switch (aiState) {
		case NONE:
			aiStateInt = 0;
			break;
		case GROUNDPATROL:
			aiStateInt = 1;
			break;
		case JUMPPATROL:
			aiStateInt = 2;
			break;
		case SCAREDAF:
			aiStateInt = 3;
			break;
		case CHASE:
			aiStateInt = 4;
			break;
		}

		aiStateValueTree.setProperty(Identifier("value"), var(aiStateInt), nullptr);

		enemySerialization.addChild(aiStateValueTree, -1, nullptr);

		return enemySerialization;
	}

	void parseFrom(ValueTree valueTree) {

		GameObject::parseFrom(valueTree);

		ValueTree aiStateValueTree = valueTree.getChildWithName(Identifier("AIState"));

		int aiStateInt = aiStateValueTree.getProperty(Identifier("value"));

		switch (aiStateInt) {
		case 0:
			changeAI(NONE);
			break;
		case 1:
			changeAI(GROUNDPATROL);
			break;
		case 2:
			changeAI(JUMPPATROL);
			break;
		case 3:
			changeAI(SCAREDAF);
			break;
		case 4:
			changeAI(CHASE);
			break;
		}

	}

private:
	double timeToSwap,timeElapsed;
	int direction;
	AIType aiState;
	float detection_radius;
	float radius;
	JUCE_LEAK_DETECTOR(EnemyObject)

};
