//
//  Model.h
//  GameEngine
//
//  Created by Tim Arterbury on 10/18/17.
//
//

#pragma once

#include "glm/glm.hpp"
#include "Mesh.h"

using namespace std;

/** Represents all the visual elements of a an object in the game world.
    This should technically be a type of Resource that is reused for multiple
    game objects. For example you could have a single instance of a Model for a
    type of enemy, and all enemy objects in the scene will reference the single
    Model.
 */
class Model
{
public:
    
    /** Default constructor creates a default square mesh.
     */
    Model()
    {
        meshes.push_back(Mesh());
        registeredWithOpenGLContext = false;
    }
    
    /** Custom constructor that allows you to specify meshes
     */
    Model (const vector<Mesh> & meshes)
    {
        this->meshes = meshes;
        registeredWithOpenGLContext = false;
    }
    
    /** Returns true of the mesh has been registered and flase otherwise
     */
    bool isRegisteredWithOpenGLContext()
    {
        return registeredWithOpenGLContext;
    }
    
    /** Initializes buffer objects & arrays for an OpenGLContext to read,
        so this Model can be draw. This must be called ONCE to register this
        Model with an OpenGLContext before it can be drawn.

        @param openGLContext    the context to render to
     */
    void registerWithOpenGLContext (OpenGLContext & openGLContext)
    {
        for (auto & mesh : meshes)
        {
            mesh.registerWithOpenGLContext(openGLContext);
        }
        registeredWithOpenGLContext = true;
    }
    
    /** Deallocates buffer objects & arrays for this Model from the OpenGLContext.
     */
    void unregisterWithOpenGLContext (OpenGLContext & openGLContext)
    {
        for (auto & mesh : meshes)
        {
            mesh.unregisterWithOpenGLContext(openGLContext);
        }
        registeredWithOpenGLContext = false;
    }
    
    /** Draws a Model to an OpenGLContext.
        @param openGLContext    the context to render to
     */
    void drawModelToOpenGLContext (OpenGLContext & openGLContext)
    {
        for (auto & mesh : meshes)
        {
            mesh.drawMeshToOpenGLContext(openGLContext);
        }
    }
	float getHeight() {
		float maxY = 0, minY = 0;
		for (Mesh &m : meshes) {
			for (auto v : m.getVertices()) {
				maxY = max(v.position.y,maxY);
				minY = min(v.position.y, minY);
			}
		}

		return maxY - minY;
	}

	float getWidth() {
		float maxX = 0, minX = 0;
		for (Mesh &m : meshes) {
			for (auto v : m.getVertices()) {
				maxX = max(v.position.x, maxX);
				minX = min(v.position.x, minX);
			}
		}
		return maxX - minX;
	}

private:
    
    /** Specifies whether or not the Model has been registered with an OpenGLContext */
    bool registeredWithOpenGLContext;
    
    /** Meshes that make up the model */
    vector<Mesh> meshes;
};

