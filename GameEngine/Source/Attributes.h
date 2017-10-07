#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vertex.h"
//==============================================================================
// This class just manages the attributes that the shaders use.
// "attribute" is a special variable type modifier in the shaders which allows to pass information
// from the CPU code to the shaders. These attributes will be passed to the Vertex shader
// to define the coordinates, normal vector, color and texture coordinate of each vertex.
// Note that an attribute variable can be a scalar, a vector, a matrix, etc.
struct Attributes
{
	Attributes(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		// Call openGL functions to get the ID (a number specific to each object or variable)
		// corresponding to the attribute whose name is given as 3rd parameter.
		// This id will be used below to tell the GPU how to use them
		position = createAttribute(openGLContext, shaderProgram, "position");
		//normal = createAttribute(openGLContext, shaderProgram, "normal");
		//sourceColour = createAttribute(openGLContext, shaderProgram, "sourceColour");
		textureCoordIn = createAttribute(openGLContext, shaderProgram, "textureCoordIn");
	}

	// This method calls openGL functions to tell the GPU that some attributes will be used
	// for each vertex (see comments below) and will be passed as an array of data
	void enable(OpenGLContext& openGLContext)
	{
		if (position != nullptr)
		{
			// Tell the GPU that the first attribute will be the position attribute
			// 2nd parameter gives the number of data (3 coordinates) for this attribute
			// 3rd parameter gives their type (floating-point)
			// 4th parameter indicates they will be left as is (not normalized)
			// 5th parameter indicates the size of the array defined for each stored element (vertex)
			// 6th parameter is the offset in that array for the given attribute in current element
			openGLContext.extensions.glVertexAttribPointer(position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			openGLContext.extensions.glEnableVertexAttribArray(position->attributeID);
		}

		/*if (normal != nullptr)
		{
		// Tell the GPU that the next attribute will be the normal attribute
		// 2nd parameter gives the number of data (3 coordinates) for this attribute
		// 3rd parameter gives their type (floating-point)
		// 4th parameter indicates they will be left as is (not normalized)
		// 5th parameter indicates the size of the array defined for each stored element (vertex)
		// 6th parameter is the byte offset in that array for the given attribute in current element (0+3 float)
		openGLContext.extensions.glVertexAttribPointer(normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
		openGLContext.extensions.glEnableVertexAttribArray(normal->attributeID);
		}

		if (sourceColour != nullptr)
		{
		// Tell the GPU that the next attribute will be the color attribute
		// 2nd parameter gives the number of data (R+G+B+Alpha) for this attribute
		// 3rd parameter gives their type (floating-point)
		// 4th parameter indicates they will be left as is (not normalized)
		// 5th parameter indicates the size of the array defined for each stored element (vertex)
		// 6th parameter is the byte offset in that array for the given attribute in current element (0+3+3 float)
		openGLContext.extensions.glVertexAttribPointer(sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 6));
		openGLContext.extensions.glEnableVertexAttribArray(sourceColour->attributeID);
		}*/

		if (textureCoordIn != nullptr)
		{
			// Tell the GPU that the next attribute will be the texture coordinate attribute
			// 2nd parameter gives the number of data (x and y) for this attribute
			// 3rd parameter gives their type (floating-point)
			// 4th parameter indicates they will be left as is (not normalized)
			// 5th parameter indicates the size of the array defined for each stored element (vertex)
			// 6th parameter is the byte offset in that array for the given attribute in current element (0+3+3+4 float)
			/*
			6th PARAM WAS CHANGED SEE COMMENTED OUT VERTEX ATTRIBUTES
			*/

			openGLContext.extensions.glVertexAttribPointer(textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3D<GLfloat>)));
			openGLContext.extensions.glEnableVertexAttribArray(textureCoordIn->attributeID);
		}
	}

	// This method calls openGL functions to tell the GPU to release the resources previously used to store attributes
	void disable(OpenGLContext& openGLContext)
	{
		if (position != nullptr)       openGLContext.extensions.glDisableVertexAttribArray(position->attributeID);
		//if (normal != nullptr)         openGLContext.extensions.glDisableVertexAttribArray(normal->attributeID);
		//if (sourceColour != nullptr)   openGLContext.extensions.glDisableVertexAttribArray(sourceColour->attributeID);
		if (textureCoordIn != nullptr)  openGLContext.extensions.glDisableVertexAttribArray(textureCoordIn->attributeID);
	}

	//ScopedPointer<OpenGLShaderProgram::Attribute> position, normal, sourceColour, textureCoordIn;
	ScopedPointer<OpenGLShaderProgram::Attribute> position, textureCoordIn;
private:
	// This method calls openGL functions to get the ID (a number specific to each object or variable,
	// which is assigned by the GPU itself) corresponding to a certain attribute name, and create the
	// attribute for the OpenGL (CPU) world.
	// Basically this will allow to link a variable in the CPU code to one in the GPU (GLSL) shader.
	// Note that the variable can be a scalar, a vector, a matrix, etc.
	static OpenGLShaderProgram::Attribute* createAttribute(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shaderProgram,
		const char* attributeName)
	{
		// Get the ID
		if (openGLContext.extensions.glGetAttribLocation(shaderProgram.getProgramID(), attributeName) < 0)
			return nullptr; // Return if error
							// Create the atttribute variable
		return new OpenGLShaderProgram::Attribute(shaderProgram, attributeName);
	}

	JUCE_LEAK_DETECTOR(Attributes)

};