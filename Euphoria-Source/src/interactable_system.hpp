#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_system.hpp"

class InteractableSystem
{
public:
	InteractableSystem() {

	}

	void init(WorldSystem& world) {
		this->world = world;
	}

	void step();

	void doInteract();
private:
	WorldSystem& world;
};