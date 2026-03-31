#pragma once
#include <stdint.h>

// Data file

using Entity = uint32_t; // unique ID to identify a game object.

struct TransformComponent { float x; float y; }; // stores the physical location of an entity.
struct SpriteComponent { uint32_t* pixels; int width; int height; }; // stores the sprite of an entity.
struct KinematicComponent { float speed; }; // stores the kinematics of an entity
struct ControllerComponent { uint32_t controllerIndex; }; // stores the controller id of an entity.