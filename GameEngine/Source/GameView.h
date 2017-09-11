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

/** Represents the view of any game being rendered.
    It includes an OpenGL Renderer to render either 2D or 3D graphics and a
    GameHUD component to render elements over the top of the OpenGL Renderer.
 */
class GameView :    public Component,
                    private OpenGLRenderer, public ChangeBroadcaster
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
        gameHUD.takeThisMagicalPointer(gameObjects, wrld);
        
        // Setup GUI Overlay Label: Status of Shaders, compiler errors, etc.
        addAndMakeVisible (statusLabel);
        statusLabel.setJustificationType (Justification::topLeft);
        statusLabel.setFont (Font (14.0f));
        statusLabel.toBack();
        
        // Create game objects
        gameObjects.add(new GameObject(wrld));
        gameObjects.add(new GameObject(wrld));
        gameObjects.getLast()->translate(1.3f, 1.0f);
        gameObjects.add(new GameObject(wrld));
        gameObjects.getLast()->translate(0.0f, 1.0f);
        gameObjects.add(new GameObject(wrld));
        gameObjects.getLast()->translate(-0.3f, 1.0f);
                    
        // GameView Variables
        isEnabled = false;
        
        setOpaque(true);
        
        
        // CRAP CODE:
        addChangeListener(&gameHUD);
        gameHUD.setMaxHeightPointer(&MAX_HEIGHT);
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
        //openGLContext.extensions.glGenBuffers (1, &VBO); // Vertex Buffer Object
        //openGLContext.extensions.glGenBuffers (1, &EBO); // Element Buffer Object
        
        // Initialize Object Buffers
        for (auto gameObject : gameObjects)
        {
            openGLContext.extensions.glGenBuffers(1, &(gameObject->getVBO()));
        }
        
    }
    
    void openGLContextClosing() override
    {
        shader = nullptr;
        uniforms = nullptr;
    }
    
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
        
        // Setup the Uniforms for use in the Shader
        if (uniforms->projectionMatrix != nullptr)
            uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);
        
        if (uniforms->viewMatrix != nullptr)
        {
            // Scale and view matrix
            Matrix3D<float> scale;
            scale.mat[0] = 2.0;
            scale.mat[5] = 2.0;
            scale.mat[10] = 2.0;
            Matrix3D<float> finalMatrix = scale * getViewMatrix();
            uniforms->viewMatrix->setMatrix4 (finalMatrix.mat, 1, false);
        }

        
        
//        // Define Which Vertex Indexes Make the Square
//        GLuint indices[] = {  // Note that we start from 0!
//            0, 1, 3,   // First Triangle
//            1, 2, 3    // Second Triangle
//        };
        
        
        // Draw all the game objects
        for (auto gameObject : gameObjects)
        {
            
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, gameObject->getVBO());
            openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, gameObject->getSizeOfVertices(), gameObject->getVertices(), GL_DYNAMIC_DRAW);
            openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            openGLContext.extensions.glEnableVertexAttribArray (0);
            glDrawArrays (GL_TRIANGLES, 0, gameObject->getNumVertices()); // For just VBO's (Vertex Buffer Objects)
        }
        
        // CRAP HEIGHT:
        // GET MAX HEIGHT THO
        MAX_HEIGHT = gameObjects[1]->getHeight();

        
        

        // VBO (Vertex Buffer Object) - Bind and Write to Buffer
        //openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, VBO);
        //openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
                                                                                        // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                        // We may want GL_DYNAMIC_DRAW since it is for
                                                                                        // vertex data that will be changing alot.
        
        // EBO (Element Buffer Object) - Bind and Write to Buffer
        //openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, EBO);
        //openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
                                                                                // GL_DYNAMIC_DRAW or GL_STREAM_DRAW
                                                                                // We may want GL_DYNAMIC_DRAW since it is for
                                                                                // vertex data that will be changing alot.
       
        // Setup Vertex Attributes
        //openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        //openGLContext.extensions.glEnableVertexAttribArray (0);
        
        // Draw Vertices
        //glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For EBO's (Element Buffer Objects) (Indices)
        //glDrawArrays (GL_TRIANGLES, 0, 6); // For just VBO's (Vertex Buffer Objects)
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindVertexArray(0);
        
        // Move Physics to next step
        wrld.Step();
        
        // CRAP CODE:
        sendChangeMessage();

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
        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0f, 1.0f);\n"
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
    //GLuint VBO;
    //GLuint VAO;
    //GLuint EBO;
    
    const char* vertexShader;
    const char* fragmentShader;
    
    // Game Data Model
    OwnedArray<GameObject> gameObjects;
    WorldPhysics wrld;
    
    // JUCE Components
    GameHUD gameHUD;
    
    // DEBUGGING
    Label statusLabel;
    
    
    // CRAP CODE:
    GLfloat MAX_HEIGHT;
    
};
