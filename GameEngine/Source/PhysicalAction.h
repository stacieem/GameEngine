//
//  PhysicalAction.h
//  GameEngine
//
//  Created by Tim Arterbury on 9/21/17.
//
//

#pragma once

/** Represents an action that is happening in the game world. Used by GameObjects
    to map certain media (audio files, textures, etc.) to be triggered when a
    specific action occurs.
 */
enum PhysicalAction { collsion, inRange, death };
