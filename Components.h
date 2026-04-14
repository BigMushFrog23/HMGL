#pragma once
#include <stdint.h>
#include <string>

// Data file

using Entity = uint32_t; // unique ID to identify a game object.

struct TransformComponent { float x = 0.0f; float y = 0.0f; }; // stores the physical location of an entity.
struct SpriteComponent { uint32_t* pixels; int width = 0; int height = 0; std::string assetName; }; // stores the sprite of an entity.
struct KinematicComponent { float velocityX = 0.0f; float velocityY = 0.0f; float speed = 0.0f; }; // stores the kinematics of an entity
struct ControllerComponent { uint32_t controllerIndex; }; // stores the controller id of an entity.
struct CollisionComponent { float width = 0.0f; float height = 0.0f; bool isStatic; float xOffset = 0.0f; float yOffset = 0.0f; };