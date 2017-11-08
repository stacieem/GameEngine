//
//  Mesh.h
//  GameEngine
//
//  Created by Tim Arterbury on 10/18/17.
//
//

#pragma once

#include "glm/glm.hpp"

using namespace std;

/** Represents a vertex in 3D space with particular attributes
 */
struct Vertex {
    
    /** Vertex position */
    glm::vec3 position;
    
    /** Color at given position */
    glm::vec4 color;
    
    /** Texture coordinate */
    glm::vec2 texCoord;
    
    // For the future if we ever flip the switch to 3D
    /** Lighting normals */
    // glm::vec3 normal;
};


// EDIT THIS FURTHERRRRR
struct Texture {
    unsigned int id;
    string type;
    //aiString path;
};

/** A single visiable part of an entire Model of a GameObject
 */
class Mesh
{
public:
    
    /** Default constructor for a Mesh, just makes a basic 2D square with no
        texture.
     */
    Mesh()
    {
        // Tex coords seem a bit odd here, I would assume we would start in the
        // top left corner as the origin instead of bottom right. There may
        // be a setting in GameView OpenGL that tells it to flip the texutre
        // to match this, so that may need to be changed for this to act in
        // the natural OpenGL/computer graphics way.
        
        
        // Create default vertices
        Vertex v;
        v.position = glm::vec3 (0.5f, 0.5f, 0.0f);
        v.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        v.texCoord = glm::vec2 (1, 1);
        vertices.push_back(v);
        
        v.position = glm::vec3 (0.5f, -0.5f, 0.0f);
        v.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        v.texCoord = glm::vec2 (1, 0);
        vertices.push_back(v);
        
        v.position = glm::vec3 (-0.5f, -0.5f, 0.0f);
        v.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        v.texCoord = glm::vec2 (0, 0);
        vertices.push_back(v);
        
        v.position = glm::vec3 (-0.5f, 0.5f, 0.0f);
        v.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        v.texCoord = glm::vec2 (0, 1);
        vertices.push_back(v);
        
        
        // Create default indices (interpret these vertices as 2 triangles)
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);

        // Default to no textures
    }

    /** Custom constructor for a Mesh, allowing vertices, indices, and textures
        to be specified.
     */
    Mesh (const vector<Vertex> & vertices, const vector<unsigned int> & indices, const vector<Texture> & textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
    }
    
    /** Initializes buffer objects & arrays for an OpenGLContext to read,
        so this Mesh can be draw.
        This must be called ONCE to register this mesh with an OpenGLContext
        before it can be drawn.
     
        @param openGLContext    the context to render to
     */
    void registerWithOpenGLContext (OpenGLContext & openGLContext)
    {
        // Create buffers/arrays
        openGLContext.extensions.glGenVertexArrays(1, &VAO);
        openGLContext.extensions.glGenBuffers(1, &VBO);
        openGLContext.extensions.glGenBuffers(1, &EBO);
        
        openGLContext.extensions.glBindVertexArray(VAO);
        
        // Load data into vertex buffer
        openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        // Fill buffer data with vertices (since each Vertex is a struct, it will
        // lay out the data exactly as it is ordered in the struct)
        openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        
        // Load data into indices buffer
        openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // Set the vertex attribute pointers (this mirrors the Vertex struct and
        // tells OpenGL the order of our data so it knows hot to interpret it)
        
        // Vertex positions
        openGLContext.extensions.glEnableVertexAttribArray(0);
        openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        
        // Vertex color coords
        openGLContext.extensions.glEnableVertexAttribArray(1);
        openGLContext.extensions.glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        
        // Vertex texture coords
        openGLContext.extensions.glEnableVertexAttribArray(2);
        openGLContext.extensions.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        
        // Vertex normals
        // For the future if we ever flip the switch to 3D
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        
        // Unbind vertex array since we are done
        openGLContext.extensions.glBindVertexArray(0);
    }
    
    /** Deallocates all buffer objects & arrays for this Mesh from the OpenGLContext
     */
    void unregisterWithOpenGLContext (OpenGLContext & openGLContext)
    {
        openGLContext.extensions.glDeleteBuffers(1, &VBO);
        openGLContext.extensions.glDeleteBuffers(1, &EBO);
        openGLContext.extensions.glDeleteVertexArrays(1, &VAO);
    }
    
    /** Draws a mesh to an OpenGLContext.
        @param openGLContext    the context to render to
     */
    void drawMeshToOpenGLContext (OpenGLContext & openGLContext)
    {
        // Draw mesh
        openGLContext.extensions.glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        openGLContext.extensions.glBindVertexArray(0);
        
        // If using texture:
        // always good practice to set everything back to defaults once configured.
        //glActiveTexture(GL_TEXTURE0);
    }

	const vector<Vertex>& getVertices() {
		return vertices;
	}

private:
    
    // Mesh data
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    // OpenGL IDs to access this mesh's buffer objects
    unsigned int VAO;   // Vertex array object
    unsigned int VBO;   // Vertex buffer object
    unsigned int EBO;   // Element buffer object
};
