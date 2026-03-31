#pragma once
#include "Registry.h"
#include "AssetManager.h"
#include <string>

// builder pattern
// can chain components together into one big building instruction.

class EntityBuilder {
private:
    Registry& m_registry;
    AssetManager& m_assets;
    Entity m_entity;

public:
    EntityBuilder(Registry& reg, AssetManager& assets)
        : m_registry(reg), m_assets(assets) {
        m_entity = m_registry.CreateEntity();
    }

    EntityBuilder& WithTransform(float x, float y) {
        m_registry.transforms[m_entity] = { x, y };
        m_registry.hasTransform[m_entity] = true; // notifies the registry that this entity is now part of the Transform systems.
        return *this; // enables method chaining
    }

    // Smart Sprite assignment using the Asset Manager
    EntityBuilder& WithSprite(const std::string& assetName) {
        Texture* tex = m_assets.GetTexture(assetName);
        if (tex) {
            m_registry.sprites[m_entity] = { tex->pixels, tex->width, tex->height };
            m_registry.hasSprite[m_entity] = true;
        }
        return *this;
    }

    EntityBuilder& WithKinematics(float speed) {
        m_registry.kinematics[m_entity] = { speed };
        m_registry.hasKinematic[m_entity] = true;
        return *this;
    }

    EntityBuilder& WithController(uint32_t playerIndex) {
        m_registry.controllers[m_entity] = { playerIndex };
        m_registry.hasController[m_entity] = true;
        return *this;
    }

    Entity Build() {
        return m_entity;
    }
};