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
        objectVBOsSize = gameModel->getGameObjects().size();
        objectVBOs = new GLuint [objectVBOsSize];
        
        for (int i = 0; i < objectVBOsSize; ++i)
        {
            openGLContext.extensions.glGenBuffers(1, &objectVBOs[i]);
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
        
        // Wait for CoreEngine to signal() GameView
        renderWaitable->wait();
        
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
            scale.mat[0] = 1.0;
            scale.mat[5] = 1.0;
            scale.mat[10] = 1.0;
            Matrix3D<float> finalMatrix = scale * getViewMatrix();
            uniforms->viewMatrix->setMatrix4 (finalMatrix.mat, 1, false);
        }
        
        
        // Draw all the game objects
        int i = 0;
        for (auto & gameObject : gameModel->getGameObjects())
        {
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, objectVBOs[i]);
            openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, gameObject->getSizeOfVertices(), gameObject->getVertices(), GL_DYNAMIC_DRAW);
            openGLContext.extensions.glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            openGLContext.extensions.glEnableVertexAttribArray (0);
            glDrawArrays (GL_TRIANGLES, 0, gameObject->getNumVertices()); // For just VBO's (Vertex Buffer Objects)
            ++i;
        }
        
        
        // Reset the element buffers so child Components draw correctly
        openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
        //openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
        openGLContext.extensions.glBindVertexArray(0);
        
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
    
};
