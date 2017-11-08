//
//  ClickableObjectSelector.h
//  GameEngine
//
//  Created by Tim Arterbury on 11/3/17.
//
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Camera.h"
#include "InspectorUpdater.h"

/** Allows the user to navigate through the game world and select various objects
    to edit the game. Keeps track of the currently selected objects.
 
    Features
        - Scroll mouse xy to manipulate camera to move around in the game world.
        - Click to select objects in the GameWorld.
*/
class WorldNavigator : public Component, public InspectorUpdater, public KeyListener
{
public:
    
    WorldNavigator(CoreEngine * coreEngine)
    {
        // Set core engine to manupilate
        this->coreEngine = coreEngine;
        
        // Default to not enabled
        isEnabled = false;
        
        // Set no camera
        camera = nullptr;
        
        // Set no selected object
        selectedObject = nullptr;
        
        // Allow the navigator to be seen through
        setOpaque(false);
        
        // Set default is not copying object
        isCopyingObject = false;
        
        // Allow keypresses to be received
        setWantsKeyboardFocus(true);
        addKeyListener(this);
    }
    
    /** Enables or disables the WorldNatigator from modifying the camera
     */
    void setEnabled (bool isEnabled)
    {
        this->isEnabled = isEnabled;
    }
    
    /** Sets the camera that the WorldNavigator will manipulate.
     */
    void setCameraToHandle (Camera * camera)
    {
        this->camera = camera;
    }
    
    /** Removes the camera the the WorldNavigator would manipulate.
     */
    void removeCameraToHandle()
    {
        this->camera = nullptr;
    }
    
    /** Sets the Level that the WorldNavigator is navigating through.
     */
    void setLevelToHandle (Level * level)
    {
        this->level = level;
    }
    
    /** Removes the Level that the WorldNavigator is navigating through.
     */
    void removeLevelToHandle()
    {
        this->level = nullptr;
    }
    
    /** Gets the selected GameObject (this may be a nullptr if no object is
        currently selected)
     */
    GameObject * getSelectedObject()
    {
        return selectedObject;
    }
    
    /** Sets the selected GameObject
     */
    void setSelectedObject (GameObject * newSelectedObject)
    {
        // Deselect old renderable object if it exists
        if (selectedObject != nullptr)
            selectedObject->setRenderableIsSelected(false);
        
        // If there is a new object to be selected
        if (newSelectedObject != nullptr)
        {
            // Save new selected object and set it to being rendered as selected
            selectedObject = newSelectedObject;
            selectedObject->setRenderableIsSelected(true);
        }
        else
        {
            // Set no selected object
            selectedObject = nullptr;
        }
        
        // Update this and other inspectors
        updateInspectorsChangeBroadcaster->sendChangeMessage();
    }
    
    /** Selects & deselects GameObjects in the GameView
     */
    void mouseDown (const MouseEvent &event) override
    {
        if (isEnabled && level != nullptr)
        {
            // Get the world position from clicking on the screen
            glm::vec2 worldPosition = camera->getWorldCoordFromScreen (getWidth(), getHeight(), event.position.x, event.position.y);
            
            // Get the GameObject at the position in the world
            GameObject * objectToSelect = level->getObjectAtPosition (worldPosition);
            
            // Set the new selected object (if no object found, this deslects the last object)
            setSelectedObject(objectToSelect);
        }
    }
    
    /** Called when the mouse moves in the component
     */
//    void mouseMove (const MouseEvent &event) override
//    {
//        const int screenW = getWidth();
//        const int screenH = getHeight();
//        
//        const float squareRadius = 30.0f;
//        
//        for (GameObject * gameObject : level->getGameObjects())
//        {
//            const float halfObjW = gameObject->getRenderableObject().model->getWidth() / 2;
//            const float halfObjH = gameObject->getRenderableObject().model->getHeight() / 2;
//            const glm::vec2 objWorld = glm::vec2(gameObject->getRenderableObject().position);
//
//            glm::vec2 screen1 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x - halfObjW, objWorld.y);
//            glm::vec2 screen2 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x + halfObjW, objWorld.y);
//            glm::vec2 screen3 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x, objWorld.y + halfObjH);
//            glm::vec2 screen4 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x, objWorld.y - halfObjH);
//            
//            if (isInSquare(event.position.x, event.position.y, screen1.x, screen1.y, squareRadius) ||
//                isInSquare(event.position.x, event.position.y, screen2.x, screen2.y, squareRadius) ||
//                isInSquare(event.position.x, event.position.y, screen3.x, screen3.y, squareRadius) ||
//                isInSquare(event.position.x, event.position.y, screen4.x, screen4.y, squareRadius))
//            {
//                setMouseCursor(MouseCursor::StandardCursorType::UpDownLeftRightResizeCursor);
//                return;
//            }
//        }
//        setMouseCursor(MouseCursor::StandardCursorType::NormalCursor);
//    }
//    
//    bool isInSquare (float x, float y, float originX, float originY, float squareRadius)
//    {
//        return x <= originX - squareRadius &&
//              originX + squareRadius <= x &&
//              y <= originY - squareRadius &&
//              originY + squareRadius <= y;
//    }
    
    /** Moves and object (and selects it)
     */
    void mouseDrag (const MouseEvent &event) override
    {
        if (isEnabled && level != nullptr && selectedObject != nullptr && event.mouseWasDraggedSinceMouseDown())
        {
            // If ALT dragging, make a copy of the object
            if (event.mods.isAltDown() && isCopyingObject == false)
            {
                isCopyingObject = true;
                GameObject * newObject = level->copyObject(selectedObject);
                setSelectedObject(newObject);
            }
            
            // Get the world position from clicking on the screen
            glm::vec2 worldPosition = camera->getWorldCoordFromScreen(getWidth(), getHeight(), event.position.x, event.position.y);
            
            // Set position of the object
            selectedObject->setPositionWithPhysics(worldPosition.x, worldPosition.y);
        }
    }
    
    /** Fires when a mouse click is over
     */
    void mouseUp (const MouseEvent &event) override
    {
        // Reset object copying
        isCopyingObject = false;
    }
    
    /** Translates the Camera when the mouse wheel is scrolled in the X and Y
        directions.
     */
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override
    {
        // If enabled, handle events
        if (isEnabled && camera != nullptr)
        {
            // Calculate the incrementation of the Camera's view
            float xIncrement = SCROLL_SPEED * wheel.deltaX;
            float yIncrement = SCROLL_SPEED * wheel.deltaY;
            
            // Translate the camera
            camera->translateXY(xIncrement, -yIncrement);
        }
    }
    
    /** Paint resize handles over all of the objects
     */
    // PROBLEM:
    // This will not render above the OpenGLContext for some reason???
    // NOTE: This needs to be refreshed 30 fps with a timer, also it seems slightly
    // off from the objects
//    void paint (Graphics & g) override
//    {
//        const float squareLength = 6.0f;
//        
//        const int screenW = getWidth();
//        const int screenH = getHeight();
//        
//        g.setColour(Colour(0xFFFFFFFF));
//        
//        for (GameObject * gameObject : level->getGameObjects())
//        {
//            const float halfObjW = gameObject->getRenderableObject().model->getWidth() / 2;
//            const float halfObjH = gameObject->getRenderableObject().model->getHeight() / 2;
//            const glm::vec2 objWorld = glm::vec2(gameObject->getRenderableObject().position);
//            
//            glm::vec2 screen1 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x - halfObjW, objWorld.y);
//            glm::vec2 screen2 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x + halfObjW, objWorld.y);
//            glm::vec2 screen3 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x, objWorld.y + halfObjH);
//            glm::vec2 screen4 = camera->getScreenCoordFromViewPlaneCoord(screenW, screenH, objWorld.x, objWorld.y - halfObjH);
//            
//            g.fillRect(screen1.x - squareLength, screen1.y - squareLength, squareLength, squareLength);
//            g.fillRect(screen2.x - squareLength, screen2.y - squareLength, squareLength, squareLength);
//            g.fillRect(screen3.x - squareLength, screen3.y - squareLength, squareLength, squareLength);
//            g.fillRect(screen4.x - squareLength, screen4.y - squareLength, squareLength, squareLength);
//        }
//    }
    
    
    bool keyPressed (const KeyPress &key, Component *originatingComponent) override
    {
        // If delete key, delete object
        if(isEnabled && (key.isKeyCode(KeyPress::backspaceKey) || key.isKeyCode(KeyPress::deleteKey)) && selectedObject != nullptr)
        {
            // Delete Object
            coreEngine->deleteGameObject(selectedObject);
            
            // Deselect object and update all other inspectors
            setSelectedObject(nullptr);
            
            // Keypress was handled
            return true;
        }
        
        // Keypress was not handled
        return false;
    }
    
    
private:
    /** A constant for how the scrolling of the mouse affects movement of camera */
    const float SCROLL_SPEED = 10.0f;
    
    /** Specifies if the navigator is enabled */
    bool isEnabled;
    
    /** Camera that is being manipulated by the navigator */
    Camera * camera;
    
    /** Model to manipulate to delete objects */
    GameModel * gameModel;
    
    /** Currently viewed level */
    Level * level;
    
    /** Currently selected object */
    GameObject * selectedObject; // In future, this should be an array of selected objects
    
    /** Core Engine to call safe GameModel functions (ex: delete objects) */
    CoreEngine* coreEngine;
    
    bool isCopyingObject;
};
