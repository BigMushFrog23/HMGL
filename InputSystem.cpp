#include "InputSystem.h"
#include <cmath>

InputSystem::InputSystem(Registry& registry, InputManager& inputManager): m_registry(registry), m_inputManager(inputManager) {}

void InputSystem::Update() {
    
    for (size_t i = 0; i < m_registry.kinematics.size(); i++) {

        // only process entities that have both a Controller and Kinematics
        if (!m_registry.hasController[i] || !m_registry.hasKinematic[i]) continue;

        auto& kinematics = m_registry.kinematics[i];

        auto& controller = m_registry.controllers[i];

        kinematics.velocityX = 0.0f;
        kinematics.velocityY = 0.0f;

        // direction vector
        float dirX = 0.0f;
        float dirY = 0.0f;

        // check input direction
        if (controller.controllerIndex == 1)
        {
            if (m_inputManager.IsActionPressed("MoveUp")) {
                dirY -= 1.0f;
            }
            if (m_inputManager.IsActionPressed("MoveDown")) {
                dirY += 1.0f;
            }
            if (m_inputManager.IsActionPressed("MoveLeft")) {
                dirX -= 1.0f;
            }
            if (m_inputManager.IsActionPressed("MoveRight")) {
                dirX += 1.0f;
            }
        }

        // normalize direction vector as to not get ~1.4 speed instead of 1.
        float length = std::sqrt((dirX * dirX) + (dirY * dirY));

        if (length > 0.0f) {
            kinematics.velocityX = (dirX / length) * kinematics.speed;
            kinematics.velocityY = (dirY / length) * kinematics.speed;
        }
    }
}