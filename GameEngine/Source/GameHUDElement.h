//
//  GameHUDElement.h
//  GameEngine
//
//  Created by Tim Arterbury on 8/29/17.
//
//



class GameHUDElement
{
public:
    /** Serializes the data model of the element to be saved and realoaded
        later.
     */
    virtual ? serializeData()=0;

    /** Loads in the serialized data model from a file??
     */
    virtual void deserializeData (?)=0;

private:


};
