//
//  GameView.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/26/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GameHUD.h"
#include "GameObject.h"
#include "WorldPhysics.h"
#include "GameModel.h"
#include "Attributes.h"
#include "Vertex.h"
#include "Uniforms.h"
#include <map>
#include "RenderSwapFrame.h"
#include "TextureResourceManager.h"

/** Represents the view of any game being rendered.
    It includes an OpenGL Renderer to render either 2D or 3D graphics and a
    GameHUD component to render elements over the top of the OpenGL Renderer.
 */
class GameView :    public Component,
                    private OpenGLRenderer
{
    
public:
    GameView()
    {
        // Sets the OpenGL version to 3.2
        // This is very important, if this is not included, new shader syntax
        // will cause a compiler error.
        openGLContext.setOpenGLVersionRequired (OpenGLContext::OpenGLVersion::openGL3_2);
        
        openGLContext.setRenderer(this);
        openGLContext.attachTo(*this);
        
        // Setup GUI Overlay Label: Status of Shaders, compiler errors, etc.
        addAndMakeVisible (statusLabel);
        statusLabel.setJustificationType (Justification::topLeft);
        statusLabel.setFont (Font (14.0f));
        statusLabel.toBack();
        
        // Setup GameHUD Overlay over OpenGL
        addAndMakeVisible(gameHUD);
        setWantsKeyboardFocus(true);

        setOpaque(true);
        
        // GameView Variables
        isEnabled = false;
    }
    
    ~GameView()
    {
        // Turn off OpenGL
        openGLContext.setContinuousRepainting (false);
        openGLContext.detach();
    }
    
    /** Enables or disables the OpenGL layer of GameView. Enabling continuously
        renders and repaints with OpenGL. Disabling will set a solid colour fill
        for the component and the component will do no processing.
     */
    void setOpenGLEnabled (bool enabled)
    {
        if (enabled)
        {
            openGLContext.setContinuousRepainting(true);
            isEnabled = true;
        }
        else
        {
            openGLContext.setContinuousRepainting(false);
            isEnabled = false;
        }
    }
    
    
    // OpenGL Callbacks ========================================================
    void newOpenGLContextCreated() override
    {
        // Setup Shaders
        createShaders();

//		openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
//		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//
//		openGLContext.extensions.glGenBuffers(1, &indexBuffer);
//		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		avgMilliseconds = 0.0;
		currentTime = Time::currentTimeMillis();
		checkTime = 0;
    }
    
    void openGLContextClosing() override
    {
        shader = nullptr;
        uniforms = nullptr;

		texResourceManager.releaseTextures();
        
        
        /**
            The code below throws an error due to a misordering of object
            destructors, but is not really necessary here.
         
        !!  BUT we do need to think about where to unregister models, probably
            when we change levels we want to register/unregister all the models
            for that level
         */
        // Unregister all the Models that were being used
//        for (auto & renderableObject : renderSwapFrame->getRenderableObjects())
//        {
//            renderableObject.model->unregisterWithOpenGLContext(openGLContext);
//        }
    }
    
    void renderOpenGL() override
    {

        jassert (OpenGLHelpers::isContextActive());
        
        // Wait for CoreEngine to signal() GameView
        renderWaitable->wait();

		//Calculate frame rate
		newTime = Time::currentTimeMillis();
		deltaTime = newTime - currentTime;
		checkTime += deltaTime;
		avgMilliseconds += ((deltaTime / 1000.0) - avgMilliseconds) * 0.03;
		currentTime = Time::currentTimeMillis();

		// For every second, update the calculated frame rate
		if (checkTime > 1000) {
			checkTime = 0;
			DBG((int)(1.0 / avgMilliseconds));
		}
        
        // Setup Viewport
        const float renderingScale = (float) openGLContext.getRenderingScale();
        glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
    
        // Set background Color
		OpenGLHelpers::clear(Colour(0xFF171C1F));

		// OpenGL methods to avoid displaying pixels behind front pixels
		glEnable(GL_DEPTH_TEST);   // Enable the test
		glDepthFunc(GL_LESS);      // Do not display back pixels
								   // Using a texture to paint main OpenGL object (teapot)
		
        // This feels super jank to me??
        //openGLContext.extensions.glActiveTexture(GL_TEXTURE0); // Using texture #0
		//glEnable(GL_TEXTURE_2D);   // It's a 2-D image texture
								   // Tell the GPU to use that texture
		
		/*END TEXTURE SAMPLE*/
        
        // Enable Alpha Blending
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Use Shader Program that's been defined
        shader->use();
        
        // Set Projection Matrix
		if (uniforms->projectionMatrix != nullptr)
        {
            uniforms->projectionMatrix->setMatrix4(&projectionMatrix[0][0], 1, false);
		}
        
        // Set View Matrix
        if (uniforms->viewMatrix != nullptr)
        {
            uniforms->viewMatrix->setMatrix4(&(renderSwapFrame->getViewMatrix()[0][0]), 1, false);
        }
        
        // Draw all the game objects
        for (auto & renderableObject : renderSwapFrame->getRenderableObjects())
        {
            // Set Texture
            // IMPLEMENT
            
            // Set Model Matrix
            if (uniforms->modelMatrix != nullptr)
            {
                uniforms->modelMatrix->setMatrix4(&renderableObject.modelMatrix[0][0], 1, false);
            }
            
            // If model has not yet been registered, register it
            if (!renderableObject.model->isRegisteredWithOpenGLContext())
            {
                renderableObject.model->registerWithOpenGLContext(openGLContext);
            }
            
			OpenGLTexture* tex = texResourceManager.loadTexture(renderableObject.animationProperties.getTexture());
			
			if (tex != nullptr) {	
				tex->bind();
			}

            // Draw Model
            renderableObject.model->drawModelToOpenGLContext(openGLContext);

			if (tex != nullptr) {
				tex->unbind();
			}
        }
        
        
//        for (auto & drawableObject : renderSwapFrame->getDrawableObjects())
//        {
//			OpenGLTexture* tex = texResourceManager.loadTexture(drawableObject->getTexture());
//
//			if (tex != nullptr) {
//				tex->bind();
//			}
//			
//			// OpenGL method to specify how the image is horizontally tiled
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//			// OpenGL method to specify how the image is vertically tiled
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//			Array<Vertex> verts = drawableObject->getVertices();
//
//			openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//			openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
//
//			openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER,
//				static_cast<GLsizeiptr> (static_cast<size_t> (verts.size()) * sizeof(Vertex)),
//				verts.getRawDataPointer(), GL_STREAM_DRAW);
//
//			// Define Which Vertex Indexes Make the Square
//			GLuint indices[] = {  // Note that we start from 0!
//				0, 1, 3,   // First Triangle
//				1, 2, 3    // Second Triangle
//			};
//
//			openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//				static_cast<GLsizeiptr> (static_cast<size_t> (6) * sizeof(GLuint)),
//				indices, GL_STREAM_DRAW);
//
//			
//
//			attributes->enable(openGLContext);
//            //glDrawArrays (GL_TRIANGLES, 0, gameObject->getNumVertices()); // For just VBO's (Vertex Buffer Objects)
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//			attributes->disable(openGLContext);
//            
//
//			if (tex != nullptr) {
//				tex->unbind();
//			}
//            
//        }
        
        // THIS IS DONE BY THE DRAW METHODS OF RENDERABLE OBJS
        // Reset the element buffers so child Components draw correctly
        // Do it just for safety right now . . . .
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindVertexArray(0);
        
		
        // Signal CoreEngine that rendering is done
        coreEngineWaitable->signal();
    }
    
    
    // JUCE Callbacks ==========================================================
	void paint(Graphics &g) override
	{
	
	}
    
	void resized() override
    {
        gameHUD.setBounds(getLocalBounds());
        statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
        
        // Setup OpenGL projection matrix to render accurate aspect ratio of
        // objects regardless of aspect ratio of the OpenGLRenderer component
        const float w = 10.0f;
        const float h = w * getLocalBounds().toFloat().getAspectRatio (false);
        // Creates an orthographic (CAD-style) projection that views world space
        // coordinates from (left edge, right edge, top, bottom)
        projectionMatrix = glm::ortho(-w, w, -h, h);
    }

    // Custom Functions ========================================================

	/** Sets the Render swap frame that will be processed for logic before it
	is sent to the GameView to be rendered.
	*/
	void setRenderSwapFrame(RenderSwapFrame * swapFrame)
	{
		renderSwapFrame = swapFrame;
	}

	/** Returns the GameModel swap frame that the GameLogic is currently
	processing.
	*/
	RenderSwapFrame * getRenderSwapFrame()
	{
		return renderSwapFrame;
	}
    
    /** Sets the WaitableEvent that allows the GameView to signal the CoreEngine
     */
    void setCoreEngineWaitable(WaitableEvent * waitable)
    {
        coreEngineWaitable = waitable;
    }
    
    /** Sets the WaitableEvent that allows the GameView to be forced to wait
        until it is signaled by the CoreEngine
     */
    void setRenderWaitable(WaitableEvent * waitable)
    {
        renderWaitable = waitable;
        
    }
    
private:
    
    //==========================================================================
    // OpenGL Shader Functions
    
    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders()
    {
        vertexShader =
//        "#version 330 core\n"
//        "layout (location = 0) in vec2 position;\n"
//		"layout (location = 1) in vec2 textureCoordIn;\n"
//        "uniform mat4 projectionMatrix;\n"
//        "uniform mat4 viewMatrix;\n"
//		"out vec2 textureCoordOut;\n"
//        "\n"
//        "void main()\n"
//        "{\n"
//		"    textureCoordOut = textureCoordIn;\n"
//        "    gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0f, 1.0f);\n"
//        "}\n";
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec4 color;\n"
        "layout (location = 2) in vec2 textureCoordIn;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "out vec4 vertexColor;\n"
        "out vec2 textureCoordOut;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    textureCoordOut = textureCoordIn;\n"
        "    vertexColor = color;\n"
        "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);\n"
        "}\n";
        
        
        fragmentShader =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "in vec2 textureCoordOut;\n"
        "out vec4 color;\n"
		"uniform sampler2D uniformTexture;\n"
        "void main()\n"
        "{\n"
		//"   color = vertexColor;\n"
        "   color = texture(uniformTexture, textureCoordOut);\n"
        "}\n";
        
        ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
        String statusText;
        
        if (newShader->addVertexShader ((vertexShader))
            && newShader->addFragmentShader ((fragmentShader))
            && newShader->link())
        {
            uniforms = nullptr;
            
            shader = newShader;
            shader->use();
            
            uniforms = new Uniforms (openGLContext, *shader);
            
            statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
        }
        else
        {
            statusText = newShader->getLastError();
        }
        
        statusLabel.setText (statusText, dontSendNotification);
    }

    // Private Variables =======================================================
    
    bool isEnabled;
    
    // OpenGL Rendering
    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Uniforms> uniforms;
    glm::mat4 projectionMatrix; // We must use a local projection matrix because
                                // is must be updated when the window is resized
                                // Therefore, we will not store this in the Camera class,
                                // although it would make more sense to be a member of Camera
    RenderSwapFrame* renderSwapFrame;
    TextureResourceManager texResourceManager;
    
    // Shaders
    const char* vertexShader;
    const char* fragmentShader;
    
    // JUCE Components
    GameHUD gameHUD;
    
    // DEBUGGING
    Label statusLabel;
    
    // Synchronization with CoreEngine
    WaitableEvent* renderWaitable;
    WaitableEvent* coreEngineWaitable;

    // Time Variables (for FPS)
	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	float avgMilliseconds;
	int64 checkTime;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GameView)
};
