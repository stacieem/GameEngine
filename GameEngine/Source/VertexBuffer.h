#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vertex.h"

/*struct VertexBuffer
{
	VertexBuffer(int indicies)
	{
		// Indices are used to be more efficient. They allow for instance to define less vertices
		// when they are used more than once. For instance to draw a square using triangles (the
		// most basic shape used by GPUs) you need 2 triangles x 3 vertices, i.e. 6 vertices.
		// But in fact 2 of them are common to the 2 triangles. By indexing the vertices you can
		// significantly reduce the calls to the Vertex Shader for more complex structures.
		numIndices = indicies;

		// These will call OpenGL routines to create a vertex buffer (VBO)
		openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		Array<Vertex> vertices;

		// This will fill in the vertex buffer with the vertex info data that was stored in the OBJ file
		// Note that a default colour is defined here in case no info was given for the color.
		//createVertexListFromMesh(aShape.mesh, vertices, COLOR_BY_CPU);
		openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (vertices.size()) * sizeof(Vertex)),
			vertices.getRawDataPointer(), GL_STATIC_DRAW);

		// These will call OpenGL routines to create a buffer and store the indices in it
		openGLContext.extensions.glGenBuffers(1, &indexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr> (static_cast<size_t> (numIndices) * sizeof(juce::uint32)),
			aShape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
	}

	~VertexBuffer()
	{
		openGLContext.extensions.glDeleteBuffers(1, &vertexBuffer);
		openGLContext.extensions.glDeleteBuffers(1, &indexBuffer);
	}

	void bind(OpenGLContext& context)
	{
		// Tell the GPU to use the specified vertex and index buffers for next frame
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	}

	GLuint vertexBuffer, indexBuffer;
	int numIndices;
	OpenGLContext& openGLContext;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VertexBuffer)
};*/