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
#include "GameModel.h"
#include "Attributes.h"
#include "Vertex.h"
#include "Uniforms.h"
#include <map>
#include "RenderSwapFrame.h"

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
        
        addAndMakeVisible(gameHUD);
        
        // Setup GUI Overlay Label: Status of Shaders, compiler errors, etc.
        addAndMakeVisible (statusLabel);
        statusLabel.setJustificationType (Justification::topLeft);
        statusLabel.setFont (Font (14.0f));
        statusLabel.toBack();
                    
        // GameView Variables
        isEnabled = false;
        objectVBOsSize = 0;
        
        setOpaque(true);
    }
    
    ~GameView()
    {
        // Turn off OpenGL
        openGLContext.setContinuousRepainting (false);
        openGLContext.detach();
		std::map<String, OpenGLTexture*>::iterator it;
		for (it = textureMap.begin(); it != textureMap.end(); it++)
		{
			delete it->second;
		}
		openGLContext.extensions.glDeleteBuffers(1, &vertexBuffer);
		openGLContext.extensions.glDeleteBuffers(1, &indexBuffer);
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

		/*SAMPLE TEXTURE LOADING**/
		String filePath = File::getCurrentWorkingDirectory().getFullPathName() + "\\textures\\p2_stand.png";
		File f = filePath;

		Image textureImage = ImageFileFormat::loadFrom(f); //ImageCache::getFromMemory (TEXTURE_DATA);
														   // Image must have height and width equal to a power of 2 pixels to be more efficient
														   // when used with older GPU architectures
		if (!(isPowerOfTwo(textureImage.getWidth()) && isPowerOfTwo(textureImage.getHeight())))
			textureImage = textureImage.rescaled(jmin(1024, nextPowerOfTwo(textureImage.getWidth())),
				jmin(1024, nextPowerOfTwo(textureImage.getHeight())));

		// Use that image as a 2-D texture for the object that will be painted
		OpenGLTexture* tex = new OpenGLTexture();
		
		tex->loadImage(textureImage);

		textureMap["Kenny"] = tex;



		filePath = File::getCurrentWorkingDirectory().getFullPathName() + "\\textures\\flower.jpg";
		f = filePath;

		textureImage = ImageFileFormat::loadFrom(f); //ImageCache::getFromMemory (TEXTURE_DATA);
														   // Image must have height and width equal to a power of 2 pixels to be more efficient
														   // when used with older GPU architectures
		if (!(isPowerOfTwo(textureImage.getWidth()) && isPowerOfTwo(textureImage.getHeight())))
			textureImage = textureImage.rescaled(jmin(1024, nextPowerOfTwo(textureImage.getWidth())),
				jmin(1024, nextPowerOfTwo(textureImage.getHeight())));

		tex = new OpenGLTexture();

		tex->loadImage(textureImage);

		textureMap["Flower"] = tex;


		/*END SAMPLE TEXTURE LOADING*/

		openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		openGLContext.extensions.glGenBuffers(1, &indexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		avgMilliseconds = 0.0;
		currentTime = Time::currentTimeMillis();
		checkTime = 0;
    }
    
    void openGLContextClosing() override
    {
        shader = nullptr;
        uniforms = nullptr;
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

		// TESTTTTT
		// For every second, update the calculated frame rate
		if (checkTime > 1000) {
			checkTime = 0;
			DBG((int)(1.0 / avgMilliseconds));
			// FIX: Update the frame rate in the GameModel
		}
        
        // Setup Viewport
        const float renderingScale = (float) openGLContext.getRenderingScale();
        glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
    
        // Set background Color
		OpenGLHelpers::clear(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

		/*TEXTURE SAMPLE*/

		// OpenGL methods to avoid displaying pixels behind front pixels
		glEnable(GL_DEPTH_TEST);   // Enable the test
		glDepthFunc(GL_LESS);      // Do not display back pixels
								   // Using a texture to paint main OpenGL object (teapot)
		openGLContext.extensions.glActiveTexture(GL_TEXTURE0); // Using texture #0
		glEnable(GL_TEXTURE_2D);   // It's a 2-D image texture
								   // Tell the GPU to use that texture
		


		/*END TEXTURE SAMPLE*/
        
        // Enable Alpha Blending
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Use Shader Program that's been defined
        shader->use();

		if (uniforms->demoTexture != nullptr)
		{
			uniforms->demoTexture->set((GLint)0);
		}
        
        // Setup the Uniforms for use in the Shader
		if (uniforms->projectionMatrix != nullptr) {
			uniforms->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);
		}
        
        if (uniforms->viewMatrix != nullptr)
        {
            // Scale and view matrix
            Matrix3D<float> scale;
            scale.mat[0] = 1.0;
            scale.mat[5] = 1.0;
            scale.mat[10] = 1.0;
            Matrix3D<float> finalMatrix = scale * getViewMatrix();
            uniforms->viewMatrix->setMatrix4 (finalMatrix.mat, 1, false);
        }
        
		attributes = new Attributes(openGLContext, *shader);
        
        // Draw all the game objects
        int i = 0;
        for (auto & gameObject : renderSwapFrame->getDrawableObjects())
        {

			textureMap[gameObject->getTexture()]->bind();

			
			// OpenGL method to specify how the image is horizontally tiled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			// OpenGL method to specify how the image is vertically tiled
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			Array<Vertex> verts = gameObject->getVertices();

			openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

			openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr> (static_cast<size_t> (verts.size()) * sizeof(Vertex)),
				verts.getRawDataPointer(), GL_STREAM_DRAW);

			// Define Which Vertex Indexes Make the Square
			GLuint indices[] = {  // Note that we start from 0!
				0, 1, 3,   // First Triangle
				1, 2, 3    // Second Triangle
			};

			openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				static_cast<GLsizeiptr> (static_cast<size_t> (6) * sizeof(GLuint)),
				indices, GL_STREAM_DRAW);

			

			attributes->enable(openGLContext);
            //glDrawArrays (GL_TRIANGLES, 0, gameObject->getNumVertices()); // For just VBO's (Vertex Buffer Objects)
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			attributes->disable(openGLContext);
            ++i;
        }
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindVertexArray(0);
        
		

        // Signal CoreEngine that rendering is done
        coreEngineWaitable->signal();
    }
    
    
    // JUCE Callbacks ==========================================================
    void resized() override
    {
        gameHUD.setBounds(getLocalBounds());
        statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
    }
    
    void paint(Graphics & g) override
    {}
    
    
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

    /** Calculates and returns the Projection Matrix.
     */
    Matrix3D<float> getProjectionMatrix() const
    {
        float w = 1.0f / (0.5f + 0.1f);
        float h = w * getLocalBounds().toFloat().getAspectRatio (false);
        return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
    }
    
    /** Calculates and returns the View Matrix.
     */
    Matrix3D<float> getViewMatrix() const
    {
        Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
        //Matrix3D<float> rotationMatrix = draggableOrientation.getRotationMatrix();
        
        return /*rotationMatrix **/ viewMatrix;
    }
    
    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders()
    {
        vertexShader =
        "#version 330 core\n"
        "attribute vec2 position;\n"
		"attribute vec2 textureCoordIn;\n"
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 viewMatrix;\n"
		"varying vec2 textureCoordOut;\n"
        "\n"
        "void main()\n"
        "{\n"
		"    textureCoordOut = textureCoordIn;\n"
        "    gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0f, 1.0f);\n"
        "}\n";
        
        // Base Fragment-Shader paints the object green.
        fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
		"varying vec4 destinationColour;\n"
		"varying vec2 textureCoordOut;\n"
		"uniform sampler2D demoTexture;\n"
        "void main()\n"
        "{\n"
		"   gl_FragColor = texture2D(demoTexture, textureCoordOut);\n"
        "}\n";
		//gl_FragColor = texture2D(demoTexture, textureCoordOut);\n"
        //"	color = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"
        ScopedPointer<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
        String statusText;
        
        if (newShader->addVertexShader ((vertexShader))
            && newShader->addFragmentShader ((fragmentShader))
            && newShader->link())
        {
            uniforms = nullptr;
            
            shader = newShader;
            shader->use();
            
            uniforms   = new Uniforms (openGLContext, *shader);
            
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
    GLuint * objectVBOs;
    int objectVBOsSize;
    //GLuint VAO;
    //GLuint EBO;
    
    const char* vertexShader;
    const char* fragmentShader;
    
    // JUCE Components
    GameHUD gameHUD;
    
    // DEBUGGING
    Label statusLabel;
    
    
    WaitableEvent* renderWaitable;
    WaitableEvent* coreEngineWaitable;

	ScopedPointer<Attributes> attributes;
	GLuint vertexBuffer, indexBuffer;
	std::map<String, OpenGLTexture*> textureMap;
	RenderSwapFrame* renderSwapFrame;

	int64 newTime;
	int64 currentTime;
	int64 deltaTime;
	float avgMilliseconds;
	int64 checkTime;
};
