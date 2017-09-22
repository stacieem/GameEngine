#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
// This class manages the uniform values that the shaders use.
struct Uniforms
{
	Uniforms(OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
	{
		projectionMatrix = createUniform(openGLContext, shaderProgram, "projectionMatrix");
		viewMatrix = createUniform(openGLContext, shaderProgram, "viewMatrix");
		demoTexture = createUniform(openGLContext, shaderProgram, "demoTexture");
	}

	ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, demoTexture;
	//ScopedPointer<OpenGLShaderProgram::Uniform> lightPosition;

private:
	static OpenGLShaderProgram::Uniform* createUniform(OpenGLContext& openGLContext,
		OpenGLShaderProgram& shaderProgram,
		const char* uniformName)
	{
		if (openGLContext.extensions.glGetUniformLocation(shaderProgram.getProgramID(), uniformName) < 0)
			return nullptr;

		return new OpenGLShaderProgram::Uniform(shaderProgram, uniformName);
	}
};