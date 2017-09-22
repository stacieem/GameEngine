#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "Vertex.h"


class DrawableObject {
public:

	DrawableObject() {
		// Default vertices and texture coordinates
		vertices.add(new Vertex(Vector3D<GLfloat>(0.5f, 0.5f, 0.0f), 1, 1));
		vertices.add(new Vertex(Vector3D<GLfloat>(0.5f, -0.5f, 0.0f), 1, 0));
		vertices.add(new Vertex(Vector3D<GLfloat>(-0.5f, -0.5f, 0.0f), 0, 0));
		vertices.add(new Vertex(Vector3D<GLfloat>(-0.5f, 0.5f, 0.0f), 0, 1));
	}

	DrawableObject(const DrawableObject& obj) {
		textureName = obj.textureName;

		for (auto & v : obj.vertices)
		{
			vertices.add(new Vertex(*v));
		}
		
	}

	DrawableObject(Array<Vertex> verts, String textureName) {
		textureName = textureName;

		for (auto & v : verts)
		{
			vertices.add(new Vertex(v));
		}

	}

	~DrawableObject() {

	}

	void setVertices(Array<Vertex> vertexArray) {
		vertices.clear();
		for (auto & obj : vertexArray)
		{
			vertices.add(new Vertex(obj));
		}
		
	}

	Array<Vertex> getVertices()
	{
		Array<Vertex> glVerticesArray;
		// Calculate GLVertices
		for (int i = 0; i < vertices.size(); i++)
		{
			Vertex v = *vertices[i];

			glVerticesArray.add(v);

		}

		return glVerticesArray;
	}

	/**
	* Set the name of the texture to use for this object. Currently,
	* all textures are loaded at runtime in GameView
	*/
	void setTexture(String tex) {
		textureName = tex;
	}

	String getTexture() {
		return textureName;
	}


private:

	OwnedArray<Vertex> vertices;

	String textureName;

};