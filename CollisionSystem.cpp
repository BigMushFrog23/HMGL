#include "CollisionSystem.h"
#include "AssetManager.h"
#include <iostream>

// we initialize the constructor
CollisionSystem::CollisionSystem(Registry& registry, AssetManager& assets) : m_registry(registry), m_assets(assets) {}

void CollisionSystem::Update()
{

	// reset texture of collision objects
	for (size_t i = 0; i < m_registry.collisions.size(); i++) {
		if (m_registry.hasCollision[i] && m_registry.hasSprite[i]) {
			Texture* original = m_assets.GetTexture(m_registry.sprites[i].assetName);
			m_registry.sprites[i].pixels = original->pixels.get();
		}
	}

	m_spatialGrid.clear();

	for (size_t i = 0; i < m_registry.collisions.size(); ++i)
	{
		if (!m_registry.hasCollision[i] || !m_registry.hasTransform[i]) continue;

		auto& transform = m_registry.transforms[i];
		auto& collision = m_registry.collisions[i];

		// we calculate the AABB Collisions (rectangles/squares)
		float minX = transform.x + collision.xOffset;
		float maxX = minX + collision.width;
		float minY = transform.y + collision.yOffset;
		float maxY = minY + collision.height;

		// convert to cell coordinates
		int gridMinX = static_cast<int>(minX / CELL_SIZE);
		int gridMaxX = static_cast<int>(maxX / CELL_SIZE);
		int gridMinY = static_cast<int>(minY / CELL_SIZE);
		int gridMaxY = static_cast<int>(maxY / CELL_SIZE);

		for (int x = gridMinX; x <= gridMaxX; x++)
		{
			for (int y = gridMinY; y <= gridMaxY; y++)
			{

				uint64_t cellKey = (static_cast<uint64_t>(static_cast<uint32_t>(x)) << 32) | static_cast<uint32_t>(y);

				m_spatialGrid[cellKey].push_back(static_cast<uint32_t>(i));
			}
		}
	}

	CheckGridCollisions();
}

void CollisionSystem::CheckGridCollisions()
{
	Texture* hitTex = m_assets.GetTexture("HitTex");
	if (!hitTex) return;

	for (auto const& [cellKey, entitiesInCell] : m_spatialGrid)
	{
		if (entitiesInCell.size() < 2) continue;

		for (size_t i = 0; i < entitiesInCell.size(); i++)
		{
			for (size_t j = i + 1; j < entitiesInCell.size(); j++)
			{

				uint32_t entityA = entitiesInCell[i];
				uint32_t entityB = entitiesInCell[j];

				auto& transformA = m_registry.transforms[entityA];
				auto& collisionA = m_registry.collisions[entityA];

				auto& transformB = m_registry.transforms[entityB];
				auto& collisionB = m_registry.collisions[entityB];

				bool isColliding = (
					transformA.x + collisionA.xOffset < transformB.x + collisionB.xOffset + collisionB.width &&
					transformA.x + collisionA.xOffset + collisionA.width > transformB.x + collisionB.xOffset &&
					transformA.y + collisionA.yOffset < transformB.y + collisionB.yOffset + collisionB.height &&
					transformA.y + collisionA.yOffset + collisionA.height > transformB.y + collisionB.yOffset
					);

				if(isColliding)
				{
					// if colliding, override the pixels to the hit texture (for now)
					m_registry.sprites[entityA].pixels = hitTex->pixels.get();
					m_registry.sprites[entityB].pixels = hitTex->pixels.get();
				}

			}
		}
	}
}