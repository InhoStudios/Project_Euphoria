#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_system.hpp"

class InteractableSystem
{
public:
	InteractableSystem() {
	}

	void init() {

	}

	void step();

	void doInteract(Entity e);
private:
};