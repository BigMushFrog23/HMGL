#pragma once
#include "Registry.h"
#include "Components.h"
#include "AssetManager.h"
#include <unordered_map>
#include <vector>
#include <string>

class CollisionSystem
{
private:
	Registry& m_registry;
	AssetManager& m_assets;

	std::unordered_map<uint64_t, std::vector<uint32_t>> m_spatialGrid;

	const int CELL_SIZE = 50;

public:
	
	CollisionSystem(Registry& registry, AssetManager& assets);

	void Update();
	void CheckGridCollisions();
};