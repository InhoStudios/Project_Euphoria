#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
bool collides_at(Entity e, Entity o, vec2 at={ 0.f, 0.f });
class PhysicsSystem
{
public:
	void step(float elapsed_ms);

	void doMobInput(float elapsed_ms);

	void doPhysicsCollisions(float elapsed_ms);
	void doGravity(float elapsed_ms);

	PhysicsSystem()
	{
	}
};