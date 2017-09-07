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
/** Represents the view of any game being rendered.
    It includes an OpenGL Renderer to render either 2D or 3D graphics and a
    GameHUD component to render elements over the top of the OpenGL Renderer.
 */
class GameView :    public Component,
                    private OpenGLRenderer
{
    
public:		
	b2Vec2 gravity;
	b2World world;
	float32 timeStep;	b2Body* body;	b2Body* body2;	juce::int32 velocityIterations;
	juce::int32 positionIterations;

    GameView() : gravity(0.0f, -10.0f), world(gravity)
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
        
        setOpaque(true);

		/*
		Startin with box2d: http://box2d.org/manual.pdf
		*/

		/*
			create the body first, giving it a position
		*/
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -11.0f);

		/*
			body is created in the world
		*/
		b2Body* groundBody = world.CreateBody(&groundBodyDef);		
		/*
			create a shape(box for this instance)
		*/
		b2PolygonShape groundBox;
		groundBox.SetAsBox(50.0f, 10.0f);				/*			assign the box shape and density to the body			When you attach a shape to a body using a fixture, the shape’s coordinates become local to the body. So
			when the body moves, so does the shape.		*/		groundBody->CreateFixture(&groundBox, 0.0f);		/*			DYNAMIC			create the body first(set it to dynamic this time)		*/
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(0.0f, 3.0f);

		b2BodyDef bodyDef2;
		bodyDef2.type = b2_dynamicBody;
		bodyDef2.position.Set(0.0f, 4.0f);
		/*
			body is created in the world
		*/
		body = world.CreateBody(&bodyDef);
		body2 = world.CreateBody(&bodyDef2);
		/*
			create shape(box for this instance)
		*/
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(.25f, .25f);
		b2PolygonShape dynamicBox2;
		dynamicBox2.SetAsBox(.15f, .15f);		/*			define fixtures physical properties		*/		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		fixtureDef.restitution = .9f;

		b2FixtureDef fixtureDef2;
		fixtureDef2.shape = &dynamicBox2;
		fixtureDef2.density = 1.0f;
		fixtureDef2.friction = 0.3f;
		fixtureDef2.restitution = 0.3f;
		/*
			assign the shape and its properties to the body
		*/
		body->CreateFixture(&fixtureDef);
		body2->CreateFixture(&fixtureDef2);		timeStep = 1.0f / 60.0f;		velocityIterations = 6;
		positionIterations = 2;
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
        
        // Setup Buffer Objects
        openGLContext.extensions.glGenBuffers (1, &VBO); // Vertex Buffer Object
        openGLContext.extensions.glGenBuffers (1, &EBO); // Element Buffer Object
    }
    
    void openGLContextClosing() override
    {
        shader = nullptr;
        uniforms = nullptr;
    }
    
	/*
	
		main loop
	*/
    void renderOpenGL() override
    {
        jassert (OpenGLHelpers::isContextActive());



        // Setup Viewport
        const float renderingScale = (float) openGLContext.getRenderingScale();
        glViewport (0, 0, roundToInt (renderingScale * getWidth()), roundToInt (renderingScale * getHeight()));
    
        // Set background Color
        OpenGLHelpers::clear (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
        // Enable Alpha Blending
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Use Shader Program that's been defined
        shader->use();

		        /*
			Box2d logic		
		*/
		
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		b2Vec2 position2 = body2->GetPosition();
		float32 angle2 = body2->GetAngle();
		DBG(position.x << " " << position.y << " " << angle);

        // Define Vertices for a Square (the view plane)
      /*  GLfloat vertices[] = {
			position.x + .25f,   position.y - .25f,  0.0f,  // Top Right	0
			position.x + .25f,  position.y + .25f,  0.0f,  // Bottom Right	1
			position.x -.25f, position.y +.25f,  0.0f,  // Bottom Left		2
			position.x - .25f,  position.y - .25f,  0.0f,   // Top Left		3

			position2.x + .15f,   position2.y - .15f,  0.0f,  // Top Right	4
			position2.x + .15f,  position2.y + .15f,  0.0f,  // Bottom Right5
			position2.x - .15f, position2.y + .15f,  0.0f,  // Bottom Left	6
			position2.x - .15f,  position2.y - .15f,  0.0f   // Top Left	7
        };
		
        // Define Which Vertex Indexes Make the Square
        GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3,   // First Triangle
            1, 2, 3,   // Second Triangle
			4, 5, 7,   // First Triangle
			5, 6, 7    // Second Triangle
        };
		*/

		GLfloat vertices1[] = {
			position.x + .25f,   position.y - .25f,  0.0f,  // Top Right	0
			position.x + .25f,  position.y + .25f,  0.0f,  // Bottom Right	1
			position.x - .25f, position.y + .25f,  0.0f,  // Bottom Left		2
			position.x - .25f,  position.y - .25f,  0.0f   // Top Left		3
		};
		GLfloat vertices2[] = {
			position2.x + .15f,   position2.y - .15f,  0.0f,  // Top Right	4
			position2.x + .15f,  position2.y + .15f,  0.0f,  // Bottom Right5
			position2.x - .15f, position2.y + .15f,  0.0f,  // Bottom Left	6
			position2.x - .15f,  position2.y - .15f,  0.0f   // Top Left	7
		};

		// Define Which Vertex Indexes Make the Square
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3,   // First Triangle
			1, 2, 3   // Second Triangle
		};

        // VBO (Vertex Buffer Object) - Bind and Write to Buffer
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, VBO);
        openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_DYNAMIC_DRAW);
                                                                                        // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                        // We may want GL_DYNAMIC_DRAW since it is for
                                                                                        // vertex data that will be changing alot.
        
        // EBO (Element Buffer Object) - Bind and Write to Buffer
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
        openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
                                                                                // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                // We may want GL_DYNAMIC_DRAW since it is for
                                                                                // vertex data that will be changing alot.
       
        // Setup Vertex Attributes
        openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        openGLContext.extensions.glEnableVertexAttribArray (0);
        
        // Draw Vertices
        glDrawElements (GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0); // For EBO's (Element Buffer Objects) (Indices)


																			// VBO (Vertex Buffer Object) - Bind and Write to Buffer
		openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, VBO);
		openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
		// GL_DYNAMIC_DRAW or GL_STREAM_DRAW
		// We may want GL_DYNAMIC_DRAW since it is for
		// vertex data that will be changing alot.

		// EBO (Element Buffer Object) - Bind and Write to Buffer
		openGLContext.extensions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		openGLContext.extensions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
		// GL_DYNAMIC_DRAW or GL_STREAM_DRAW
		// We may want GL_DYNAMIC_DRAW since it is for
		// vertex data that will be changing alot.

		// Setup Vertex Attributes
		openGLContext.extensions.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		openGLContext.extensions.glEnableVertexAttribArray(0);

		// Draw Vertices
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0); // For EBO's (Element Buffer Objects) (Indices)

        //glDrawArrays (GL_TRIANGLES, 0, 6); // For just VBO's (Vertex Buffer Objects)
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindVertexArray(0);
		

		

    }
    
    
    // JUCE Callbacks ==========================================================
    void resized() override
    {
        gameHUD.setBounds(getLocalBounds());
        statusLabel.setBounds (getLocalBounds().reduced (4).removeFromTop (75));
    }
    
    void paint(Graphics & g) override
    {}
    
    
private:
    
    //==========================================================================
    // OpenGL Shader Functions

    /** Calculates and returns the Projection Matrix.
     */
//    Matrix3D<float> getProjectionMatrix() const
//    {
//        float w = 1.0f / (0.5f + 0.1f);
//        float h = w * getLocalBounds().toFloat().getAspectRatio (false);
//        return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 30.0f);
//    }
    
    /** Calculates and returns the View Matrix.
     */
//    Matrix3D<float> getViewMatrix() const
//    {
//        Matrix3D<float> viewMatrix (Vector3D<float> (0.0f, 0.0f, -10.0f));
//        Matrix3D<float> rotationMatrix = draggableOrientation.getRotationMatrix();
//        
//        return rotationMatrix * viewMatrix;
//    }
    
    /** Loads the OpenGL Shaders and sets up the whole ShaderProgram
     */
    void createShaders()
    {
        vertexShader =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(position, 0.0f, 1.0f);\n"
        "}\n";
        
        // Base Fragment-Shader paints the object green.
        fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "    color = vec4 (0.0f, 1.0f, 1.0f, 1.0f);\n"
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
        }
        
        ScopedPointer<OpenGLShaderProgram::Uniform> projectionMatrix, viewMatrix;
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
    
    
    bool isEnabled;
    
    // OpenGL Rendering
    OpenGLContext openGLContext;
    ScopedPointer<OpenGLShaderProgram> shader;
    ScopedPointer<Uniforms> uniforms;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    
    const char* vertexShader;
    const char* fragmentShader;
    
    // JUCE Components
    GameHUD gameHUD;
    
    // DEBUGGING
    Label statusLabel;
    
};
