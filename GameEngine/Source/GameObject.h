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
    GameObject(WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.world)
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
    GLuint & getVBO()
    {
        return VBO;
    }
    
    void translate (GLfloat x, GLfloat y)
    {
        Vector3D<GLfloat> transformation (x, y, 0.0);
        
        position += transformation;
        
		physicsProperties.translate(x, y);
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
    
    // GLAttributes
    GLuint VBO; // The ID of the OpenGL Vertex buffer that has been registered
    
    OwnedArray<Vector3D<GLfloat>> vertices; // The vertices from the origin
    ScopedPointer<GLfloat> glVertices;
    
    Vector3D<GLfloat> position;
    //Matrix3D<GLfloat> transformations;
    
    PhysicsProperties physicsProperties;
    
    //Vector3D<GLfloat> color;
    
    // Texture
    
    //PhysicsProperties
        //bool isSolid;
    
};
