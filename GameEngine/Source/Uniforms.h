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
        modelMatrix = createUniform(openGLContext, shaderProgram, "modelMatrix");
		isLeftAnimation = createUniform(openGLContext, shaderProgram, "isLeftAnimation");
        isSelectedObject = createUniform(openGLContext, shaderProgram, "isSelectedObject");
	}

    ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> viewMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> modelMatrix;
    ScopedPointer<OpenGLShaderProgram::Uniform> isLeftAnimation;
    ScopedPointer<OpenGLShaderProgram::Uniform> isSelectedObject;

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
