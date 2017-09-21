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
		texture.release();
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
		texture.loadImage(textureImage);

		openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		openGLContext.extensions.glGenBuffers(1, &indexBuffer);
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        
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
        renderWaitable->wait(500);
        
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
		texture.bind();
		// OpenGL method to specify how the image is horizontally tiled
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// OpenGL method to specify how the image is vertically tiled
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


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
        for (auto & gameObject : gameModel->getGameObjects())
        {

			GLfloat * vPtr = gameObject->getVertices();

			Array<Vertex> verts;
			Vertex vert;
			vert.position[0] = vPtr[0];
			vert.position[1] = vPtr[1];
			vert.position[2] = vPtr[2];

			vert.texCoord[0] = 1;
			vert.texCoord[1] = 1;
			verts.add(vert);

			Vertex vert2;
			vert2.position[0] = vPtr[3];
			vert2.position[1] = vPtr[4];
			vert2.position[2] = vPtr[5];
			vert2.texCoord[0] = 1;
			vert2.texCoord[1] = 0;
			verts.add(vert2);

			Vertex vert3;
			vert3.position[0] = vPtr[6];
			vert3.position[1] = vPtr[7];
			vert3.position[2] = vPtr[8];
			vert3.texCoord[0] = 0;
			vert3.texCoord[1] = 0;
			verts.add(vert3);

			Vertex vert4;
			vert4.position[0] = vPtr[9];
			vert4.position[1] = vPtr[10];
			vert4.position[2] = vPtr[11];
			vert4.texCoord[0] = 0;
			vert4.texCoord[1] = 1;
			verts.add(vert4);


			openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

			openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr> (static_cast<size_t> (verts.size()) * sizeof(Vertex)),
				verts.getRawDataPointer(), GL_STATIC_DRAW);

			// Define Which Vertex Indexes Make the Square
			GLuint indices[] = {  // Note that we start from 0!
				0, 1, 3,   // First Triangle
				1, 2, 3    // Second Triangle
			};

			openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				static_cast<GLsizeiptr> (static_cast<size_t> (6) * sizeof(GLuint)),
				indices, GL_STATIC_DRAW);

			

			attributes->enable(openGLContext);
            //glDrawArrays (GL_TRIANGLES, 0, gameObject->getNumVertices()); // For just VBO's (Vertex Buffer Objects)
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			attributes->disable(openGLContext);
            ++i;
        }
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
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
    
    /** Sets the GameModel to currently render. This is one of the frames that
        is swapped back and forth between the GameLocic and GameView
     */
    void setGameModelSwapFrame(GameModel * swapFrame)
    {
        this->gameModel = swapFrame;
    }
    
    /** Gets the GameView's current gameModel swap frame
     */
    GameModel * getGameModelSwapFrame()
    {
        return gameModel;
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
        "layout (location = 0) in vec2 position;\n"
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
    
    //==============================================================================
    // This class manages the uniform values that the shaders use.
    struct Uniforms
    {
        Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
        {
            projectionMatrix = createUniform (openGLContext, shaderProgram, "projectionMatrix");
            viewMatrix       = createUniform (openGLContext, shaderProgram, "viewMatrix");
			demoTexture = createUniform(openGLContext, shaderProgram, "demoTexture");
        }
        
        ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix, demoTexture;
        //ScopedPointer<OpenGLShaderProgram::Uniform> lightPosition;
        
        private:
        static OpenGLShaderProgram::Uniform* createUniform (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shaderProgram,
                                                            const char* uniformName)
        {
            if (openGLContext.extensions.glGetUniformLocation (shaderProgram.getProgramID(), uniformName) < 0)
            return nullptr;
            
            return new OpenGLShaderProgram::Uniform (shaderProgram, uniformName);
        }
    };

	struct Vertex
	{
		float position[3];  // To define vertex x,y,z coordinates
		//float normal[3];    // Orthogonal vector used to calculate light impact on the texture color
		//float colour[4];    // Color used for the vertex. If no other color info is given for the fragment
							// the pixel colors will be interpolated from the vertex colors
		float texCoord[2];  // A graphic image (file) can be used to define the texture of the drawn object.
							// This 2-D vector gives the coordinates in the 2-D image file corresponding to
							// the pixel color to be drawn
	};

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
			
				openGLContext.extensions.glVertexAttribPointer(textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(float) * 3));
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
	};



    
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
    
    // GameModel
    GameModel* gameModel;
    WaitableEvent* renderWaitable;
    WaitableEvent* coreEngineWaitable;

	//Crap code for inital textures dev
	File f;
	Image im;
	OpenGLTexture texture;
	ScopedPointer<Attributes> attributes;
	GLuint vertexBuffer, indexBuffer;
};
