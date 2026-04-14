#include "MovementSystem.h"

MovementSystem::MovementSystem(Registry& registry) : m_registry(registry) {}

void MovementSystem::Update(float deltaTime) {

    // loop through all entities
    for (size_t i = 0; i < m_registry.transforms.size(); i++) {

        // only process entities that have BOTH a Transform and Kinematics
        if (!m_registry.hasTransform[i] || !m_registry.hasKinematic[i]) continue;

        auto& transform = m_registry.transforms[i];
        auto& kinematics = m_registry.kinematics[i];

        // apply the velocity to the position with deltaTime
        transform.x += kinematics.velocityX * deltaTime;
        transform.y += kinematics.velocityY * deltaTime;
    }
}