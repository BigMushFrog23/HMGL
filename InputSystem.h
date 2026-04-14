#pragma once

#include "Registry.h"
#include "InputManager.h"

class InputSystem {
private:
    // store references so we modify the memory address and not a copy of the object
    Registry& m_registry;
    InputManager& m_inputManager;

public:
    
    InputSystem(Registry& registry, InputManager& inputManager);

    void Update();
};