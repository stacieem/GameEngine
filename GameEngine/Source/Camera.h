//
//  Camera.h
//  GameEngine
//
//  Created by Tim Arterbury on 10/18/17.
//
//

#pragma once

#include "glm/glm.hpp"

/** Represents a simple 2D camera that can be positioned.
    
    Future WishList:
        - Rotations
        - Full 3D usage
 */
class Camera
{
public:
    
    /** Initializes the view matrix
     */
    Camera()
    {
        // Initialize to identity
        viewMatrix = glm::mat4(1.0);
        
        // Set the Z position of camera to be at 0 since this is a flat scene
        viewMatrix[3][2] = 0.0f;
        
        // Set default scale
        setScale(1.0f);
    }
    
    /** Gets the view matrix of the Camera
     */
    glm::mat4 getViewMatrix()
    {
        return viewMatrix;
    }
    
    /** Gets the projection matrix of the Camera
     */
    glm::mat4 getProjectionMatrix()
    {
        return projectionMatrix;
    }
    
    /** Gets the xyz position of the camera.
     */
    glm::vec3 getPosition()
    {
        return glm::vec3(viewMatrix[3][0], viewMatrix[3][1], viewMatrix[3][2]);
    }
    
    /** Sets xyz position of the camera. Since this camera is only really 2D
        right now, we don't need a a look vector.
     */
    void setPosition(glm::vec3 position)
    {
        viewMatrix[3][0] = position.x;
        viewMatrix[3][1] = position.y;
        viewMatrix[3][2] = position.z;
    }
    
    /** Sets x position of the camera. For a side scroller, we will use this to
        set the position of the camera based on the player's movement.
     */
    void setPositionXY (float x, float y)
    {
        viewMatrix[3][0] = x;
        viewMatrix[3][1] = y;
    }
    
    
    /** Translates the camera by x and y coordinates.
     */
    void translateXY (float x, float y)
    {
        viewMatrix[3][0] += x;
        viewMatrix[3][1] += y;
    }

    
    /** Sets the scale or zoom of the camera.
     */
    void setScale (float scale)
    {
        viewMatrix[0][0] = scale;
        viewMatrix[1][1] = scale;
        viewMatrix[2][2] = scale;
    }
    
    /** Sets the width and height of the projection
     */
    void setProjectionWH (float w, float h)
    {
        viewPlaneWidth = 2.0f * w;
        viewPlaneHeight = 2.0f * h;
        projectionMatrix = glm::ortho(-w, w, -h, h);
    }
    
    
    glm::vec2 getWorldCoordFromScreen (int widthScreen, int heightScreen, float xScreen, float yScreen)
    {
        // YScreen should be interpreted from the top left of the screen
        
        // BAD: Eventually, this should multiply the screen point by the
        // inverse projection matrix (this way if the projection changes, so too
        // will the view plane cordinate)
        float xViewPlane = ((float) xScreen / (float) widthScreen * viewPlaneWidth) - viewPlaneWidth / 2.0f;
        float yViewPlane = -(((float) yScreen / (float) heightScreen * viewPlaneHeight) - viewPlaneHeight / 2.0f);
        
        
        // Calculate world position based on viewPlane Coord and camera position
        glm::vec3 cameraPosition = getPosition(); // ignore the z axis for now
        glm::vec2 worldPosition = glm::vec2(xViewPlane - cameraPosition.x,
                                            yViewPlane  - cameraPosition.y);
        
        return worldPosition;
    }
    
    glm::vec2 getScreenCoordFromViewPlaneCoord(int widthScreen, int heightScreen, float xWorld, float yWorld)
    {
        
        // Calculate world position based on viewPlane Coord and camera position
        glm::vec3 cameraPosition = getPosition(); // ignore the z axis for now
        
        // YScreen should be interpreted from the top left of the screen
        
        // BAD: Eventually, this should multiply the screen point by the
        // inverse projection matrix (this way if the projection changes, so too
        // will the view plane cordinate)
        float xViewPlane = xWorld + cameraPosition.x;
        float yViewPlane = yWorld + cameraPosition.y;
        
        float xScreen = ((float) widthScreen / viewPlaneWidth) * (xViewPlane + viewPlaneWidth / 2.0f);
        float yScreen = -((float) heightScreen / viewPlaneHeight * (yViewPlane - viewPlaneHeight / 2.0f));
        
        return glm::vec2(xScreen, yScreen);

    }
    
private:
    float viewPlaneHeight;
    float viewPlaneWidth;
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};
