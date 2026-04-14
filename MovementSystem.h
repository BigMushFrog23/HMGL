#pragma once
#include "Registry.h"

class MovementSystem {
private:
    Registry& m_registry;

public:
    MovementSystem(Registry& registry);

    // deltaTime to ensure frame-rate dependent movement
    void Update(float deltaTime);
};