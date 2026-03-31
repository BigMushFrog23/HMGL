#pragma once
#include <vector> // ensures cache-locality, allows the CPU to pre-fetch components (they are in a straight line in memory).
#include "Components.h"

// Database file

class Registry
{
private:
	Entity m_nextEntityId = 0;

public:

	std::vector<TransformComponent>  transforms;
	std::vector<SpriteComponent>	 sprites;
	std::vector<KinematicComponent>  kinematics;
	std::vector<ControllerComponent> controllers;

	// check if entity owns a specific piece of data without searching a map.
	std::vector<bool> hasTransform;
	std::vector<bool> hasSprite;
	std::vector<bool> hasKinematic;
	std::vector<bool> hasController;

	Entity CreateEntity()
	{
		Entity id = m_nextEntityId++;

		// dynamic resizing to keep all parallel arrays in sync, and make sure they have enough slots.
		size_t newSize = (size_t)id + 1;
		transforms.resize(newSize);
		sprites.resize(newSize);
		kinematics.resize(newSize);
		controllers.resize(newSize);

		hasTransform.resize(newSize, false);
		hasSprite.resize(newSize, false);
		hasKinematic.resize(newSize, false);
		hasController.resize(newSize, false);

		return id;
	}

	size_t GetEntityCount() const { return transforms.size(); }

};