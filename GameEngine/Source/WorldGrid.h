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
        glm::vec2 griddedPosition;
        
        // Get the number of unit lengths found in the ungridded position
        glm::vec2 unitLengthsInUngridded = ungriddedPosition / unitLength;
        
        // If the decimal values of the x value of number of unit lengths is rounded
        // to zero
        if (round (abs(unitLengthsInUngridded.x) - floor (abs(unitLengthsInUngridded.x))) == 0)
        {
            griddedPosition.x = unitLength * trunc(unitLengthsInUngridded.x);
        }
        else
        {
            if (unitLengthsInUngridded.x >= 0.0f)
                griddedPosition.x = unitLength * (1 + trunc(unitLengthsInUngridded.x));
            else
                griddedPosition.x = unitLength * (-1 + trunc(unitLengthsInUngridded.x));
        }
        
        // If the decimal values of the y value of number of unit lengths is rounded
        // to zero
        if (round (abs(unitLengthsInUngridded.y) - floor (abs(unitLengthsInUngridded.y))) == 0)
        {
            griddedPosition.y = unitLength * trunc(unitLengthsInUngridded.y);
        }
        else
        {
            if (unitLengthsInUngridded.y >= 0.0f)
                griddedPosition.y = unitLength * (1 + trunc(unitLengthsInUngridded.y));
            else
                griddedPosition.y = unitLength * (-1 + trunc(unitLengthsInUngridded.y));
        }
        
        return griddedPosition;
    }
    
private:
    float unitLength;
};
