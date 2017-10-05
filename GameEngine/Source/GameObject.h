//
//  GameObject.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/7/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vertex.h"
#include "PhysicsProperties.h"
#include "WorldPhysics.h"
#include "PhysicalAction.h"
#include <map>

/** Represents an Object that holds vertices that can be rendered by OpenGL.
 */
class GameObject
{
public:
    /** Constructs a GameObject and attatches it to the world's physics.
     */
    GameObject(WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.getWorld()), audioFile (File::getCurrentWorkingDirectory().getFullPathName() + "/Air Horn.wav")
    {
        // Default vertices and texture coordinates
        vertices.add(new Vertex(Vector3D<GLfloat>(0.5f,   0.5f,  0.0f),1,1));
		vertices.add(new Vertex(Vector3D<GLfloat>(0.5f, -0.5f, 0.0f), 1, 0));
		vertices.add(new Vertex(Vector3D<GLfloat>(-0.5f, -0.5f, 0.0f), 0, 0));
		vertices.add(new Vertex(Vector3D<GLfloat>(-0.5f, 0.5f, 0.0f), 0, 1));

        // Default mapping to an objects audio
        mapAudioFileToPhysicalAction(File(File::getCurrentWorkingDirectory().getFullPathName() + "/Air Horn.wav"), PhysicalAction::collsion);
    }

	virtual ~GameObject() {

	}

    
    /** Get the
     */
    Array<Vertex> getVertices()
    {
        // Calculate vertices based on Box2D's transformations
        
        b2Vec2 box2DPos = physicsProperties.GetPosition();
        position.x = box2DPos.x;
        position.y = box2DPos.y;
        
		Array<Vertex> glVerticesArray;
        // Calculate GLVertices
        for (int i = 0; i < vertices.size(); i++)
        {
			Vertex v = *vertices[i];
			
            v.position += position;
			glVerticesArray.add(v);
           
        }
        
        return glVerticesArray;
    }
    
    std::size_t getSizeOfVertices()
    {
        return sizeof(GLfloat) * vertices.size() * 3;
    }
    
    int getNumVertices()
    {
        return vertices.size();
    }
    
    
    void translateTo (GLfloat x, GLfloat y)
    {
        Vector3D<GLfloat> transformation (x, y, 0.0);
        
        position += transformation;
        
		physicsProperties.translateTo(x, y);
    }
    

    PhysicsProperties & getPhysicsProperties()
    {
        return physicsProperties;
    }
    
    /** Maps an audio file to play when a specific action happens to this object
        in the physical game world.
     */
    void mapAudioFileToPhysicalAction (File audioFile, PhysicalAction action)
    {
        actionToAudio.insert(std::pair<PhysicalAction, File>(action, audioFile));
    }
    
    
    /** Gets the audio to play when a specific PhysicalAction occurs in the game.
     */
    File * getAudioFileForAction(PhysicalAction action)
    {
        auto mapIterator = actionToAudio.find(action);
        if (mapIterator != actionToAudio.end())
            return &(actionToAudio.find(action)->second);
        else
            return nullptr;
    }
    
    // !FIX! Dont need this if the map stuff works
    File getAudioFile()
    {
        return audioFile;
    }
    
    
	void translateBy (GLfloat x, GLfloat y)
	{
		Vector3D<GLfloat> transformation(x, y, 0.0);

		position += transformation;

		physicsProperties.translateBy(x, y);
	}

	/**
	* Set the name of the texture to use for this object. Currently, 
	* all textures are loaded at runtime in GameView
	*/
	void setTexture(File tex) {
		textureFile = tex;
	}

	File getTexture() {
		return textureFile;
	}
    
private:
    
    // Physical Position =======================================================
	Vector3D<GLfloat> position;

	OwnedArray<Vertex> vertices;

    //Matrix3D<GLfloat> transformations;
    
    /** Physical properties associated with the object */
    PhysicsProperties physicsProperties;
    
    /** Map of in-game physics-based actions to specific audio files */
    std::map<PhysicalAction, File> actionToAudio;
    
    File audioFile;
    
	File textureFile;

//    AudioFileList files;
//    std::map<> actionToAudioMap;
//    
//    AnimationList animations;
//    std::map<> actionToAnimationMap;
    
    // Someone setting up a peice of audio to be triggered by an action:
    // actionToAudioMap.map(files[0], IN_RANGE_ACTION);
    // actionToAudioMap.map(files[1], COLLISION_ACTION);
    // actionToAudioMap.map(files[1], INTERACTION_ACTION);
    //
    // actionToAnimationMap.map (animations[0], IN_RANGE_ACTION);
    //
    // Then the corresponding audio is called in the actionTriggered calback in
    //  this object:
    //
    //  void actionTriggered (actionId) {
    //
    //      playAudio (actionToAudioMap.get(actionId));
    //      startAnimaction (actionToAnimationMap.get(actionId));
    //      // etc for all other things to do when an action is triggered . . .
    // }
    
    
    
    
    //Vector3D<GLfloat> color;
    
	JUCE_LEAK_DETECTOR(GameObject)


};
