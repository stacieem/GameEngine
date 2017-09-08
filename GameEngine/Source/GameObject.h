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

class GameObject
{
public:
    GameObject(WorldPhysics & worldPhysics) : physicsProperties (worldPhysics.world)
    {
        // Default vertices
//        vertices.add(Vector3D<GLfloat>(0.0f, 0.5f, 0.0f));
//        vertices.add(Vector3D<GLfloat>(-0.5f, -0.5f, 0.0f));
//        vertices.add(Vector3D<GLfloat>(0.5f, -0.5f, 0.0f));
        
        // Makes a triangle
        numVertices = 6;
        vertices = new GLfloat[numVertices * 3] {
            0.5f,   0.5f,  0.0f,  // Top Right
            0.5f,  -0.5f,  0.0f,  // Bottom Right
            -0.5f, -0.5f,  0.0f,  // Bottom Left
            -0.5f,   0.5f,  0.0f,  // Top Left
            -0.5f, -0.5f,  0.0f,  // Bottom Left
            0.5f,   0.5f,  0.0f  // Top Right
        };
    }
    
    GLfloat * getVertices()
    {
        // Calculate vertices based on position, transformations, etc. . .
        b2Vec2 position = physicsProperties.getBody()->GetPosition();
        
        for (int i = 0; i < numVertices * 3; i += 3)
        {
            //vertices[i] += position.x;
            vertices[i+1] += position.y;
        }

        
        return vertices.get();
    }
    
    std::size_t getSizeOfVertices()
    {
        return sizeof(GLfloat) * numVertices * 3;
    }
    
    int getNumVertices()
    {
        return numVertices;
    }
    
    GLuint * getVBOPtr()
    {
        return &VBO;
    }
    
    void translate (GLfloat x, GLfloat y)
    {
        for (int i = 0; i < numVertices * 3; i += 3)
        {
            vertices[i] += x;
            vertices[i+1] += y;
        }
    }
    
private:
    
    // GLAttributes
    GLuint VBO;
    
    ScopedPointer<GLfloat> vertices;
    int numVertices;
    
    //Array<Vector3D<GLfloat>> vertices;
    Vector3D<GLfloat> position;
    Matrix3D<GLfloat> transformations;
    
    PhysicsProperties physicsProperties;
    
    //Vector3D<GLfloat> color;
    
    // Texture
    
    //PhysicsProperties
        //bool isSolid;
    
};
