#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
	float knockback, int damage, int ttl);

class CombatSystem
{
public:
	CombatSystem() { }

	void step();
private:

};