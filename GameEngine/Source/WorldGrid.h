//
//  WorldGrid.h
//  GameEngine
//
//  Created by Tim Arterbury on 11/9/17.
//
//

#pragma once

class WorldGrid
{
public:
    
    WorldGrid()
    {
        // Defaut to a grid of units of 1.0f
        unitLength = 1.0f;
    }
    
    void setGridUnitLength (float unitLength)
    {
        this->unitLength = unitLength;
    }
    
    float getUnitLength()
    {
        return unitLength;
    }
    
    glm::vec2 getGriddedPostition (glm::vec2 ungriddedPosition)
    {
        return glm::vec2(roundToClosestGridUnit(ungriddedPosition.x),
                         roundToClosestGridUnit(ungriddedPosition.y));
    }
    
private:
    
    float roundToClosestGridUnit (float distance)
    {
        return round(distance / unitLength) * unitLength;
    }
    
    float unitLength;
};
