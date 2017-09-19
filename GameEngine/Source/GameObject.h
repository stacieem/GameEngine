//
//  GameObject.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/7/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PhysicsProperties.h"
#include "WorldPhysics.h"

/** Represents an Object that holds vertices that can be rendered by OpenGL.
 */
class GameObject
{
public:
    /** Constructs a GameObject and attatches it to the world's physics.
     */
    GameObject(WorldPhysics & worldPhysics) : physicsProperties (*worldPhysics.getWorld())
    {
        // Default vertices
        vertices.add(new Vector3D<GLfloat>(0.5f,   0.5f,  0.0f));
        vertices.add(new Vector3D<GLfloat>(0.5f,  -0.5f,  0.0f));
        vertices.add(new Vector3D<GLfloat>(-0.5f, -0.5f,  0.0f));
        vertices.add(new Vector3D<GLfloat>(-0.5f,   0.5f,  0.0f));
        vertices.add(new Vector3D<GLfloat>(-0.5f, -0.5f,  0.0f));
        vertices.add(new Vector3D<GLfloat>(0.5f,   0.5f,  0.0f));
        
        // Vertices sent to OpenGL
        glVertices = new GLfloat[vertices.size() * 3];
    }
    
    /** Get the
     */
    GLfloat * getVertices()
    {
        // Calculate vertices based on Box2D's transformations
        
        b2Vec2 box2DPos = physicsProperties.GetPosition();
        position.x = box2DPos.x;
        position.y = box2DPos.y;
        
        // Calculate GLVertices
        for (int i = 0; i < vertices.size() * 3; i += 3)
        {
            glVertices[i] = vertices[i / 3]->x + position.x;
            glVertices[i + 1] = vertices[(i+1) / 3]->y + position.y;
            glVertices[i + 2] = vertices[(i+2) / 3]->z + position.z;
        }
        
        return glVertices.get();
    }
    
    std::size_t getSizeOfVertices()
    {
        return sizeof(GLfloat) * vertices.size() * 3;
    }
    
    int getNumVertices()
    {
        return vertices.size();
    }
    
    /** Get the VBO for the object.
     */
//    GLuint & getVBO()
//    {
//        return VBO;
//    }
    
    void translateTo (GLfloat x, GLfloat y)
    {
        Vector3D<GLfloat> transformation (x, y, 0.0);
        
        position += transformation;
        
		physicsProperties.translateTo(x, y);
    }
    
	void translateBy(GLfloat x, GLfloat y)
	{
		Vector3D<GLfloat> transformation(x, y, 0.0);

		position += transformation;

		physicsProperties.translateBy(x, y);
	}

    PhysicsProperties getPhysicsProperties()
    {
        return physicsProperties;
    }
    
    // CRAP CODE:
    GLfloat getHeight()
    {
        return position.y;
    }
private:
    
    // Physical Position =======================================================
    
    OwnedArray<Vector3D<GLfloat>> vertices; // The vertices from the origin
    ScopedPointer<GLfloat> glVertices;

    Vector3D<GLfloat> position;
    //Matrix3D<GLfloat> transformations;
    
    
    PhysicsProperties physicsProperties;
    
    
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
    
    // Texture
    
    //PhysicsProperties
        //bool isSolid;
    
};
