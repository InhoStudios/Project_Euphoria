// internal
#include "physics_system.hpp"
#include "world_init.hpp"

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move bug based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	doPlayerInput(elapsed_ms);
	doGravity(elapsed_ms);
	doPhysicsCollisions(elapsed_ms);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A2: HANDLE EGG UPDATES HERE
	// DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Check for collisions between all moving entities
	/*
    ComponentContainer<Motion> &motion_container = registry.motions;
	for(uint i = 0; i<motion_container.components.size(); i++)
	{
		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];
		
		// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
		for(uint j = i+1; j<motion_container.components.size(); j++)
		{
			Motion& motion_j = motion_container.components[j];
			if (collides(motion_i, motion_j))
			{
				Entity entity_j = motion_container.entities[j];
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
		}
	}
	*/

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A2: HANDLE EGG collisions HERE
	// DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void PhysicsSystem::doPlayerInput(float elapsed_ms) {
	// iterate through players
	auto& player_registry = registry.players;
	for (uint i = 0; i < player_registry.size(); i++) {
		Entity playerEntity = player_registry.entities[i];
		Player& player = player_registry.get(playerEntity);
		Motion& playerMotion = registry.motions.get(playerEntity);
		Physics& physics = registry.physEntities.get(playerEntity);
		Mob& mob = registry.mobs.get(playerEntity);

		Input& input = registry.inputs.get(playerEntity);

		int hdir = input.key[KEY::RIGHT] - input.key[KEY::LEFT];

		if (hdir) {
			playerMotion.scale.x = hdir * abs(playerMotion.scale.x);
		}

		physics.targetVelocity.x = hdir * mob.moveSpeed;

		if (input.key_press[KEY::JUMP] && !physics.inAir) {
			physics.targetVelocity.y = -mob.jumpSpeed;
			physics.velocity.y = physics.targetVelocity.y;
		}

		if (input.key_release[KEY::JUMP] && physics.inAir) {
			physics.targetVelocity.y = 0;
			physics.velocity.y = 0;
		}
	}
}

void PhysicsSystem::doGravity(float elapsed_ms) {

	auto& motion_registry = registry.motions;
	auto& gravity_registry = registry.gravEntities;
	auto& physics_registry = registry.physEntities;
	auto& mob_registry = registry.mobs;

	for (uint i = 0; i < gravity_registry.size(); i++) {
		float step_seconds = elapsed_ms / 1000.f;
		Entity entity = gravity_registry.entities[i];

		Gravity& gravity = gravity_registry.components[i];
		Physics& physics = physics_registry.get(entity);

		if (abs(physics.targetVelocity.y) <= gravity.terminalVelocity &&
			abs(physics.velocity.y) <= gravity.terminalVelocity) {
			physics.targetVelocity.y += step_seconds * gravity.grav;
			physics.velocity.y += step_seconds * gravity.grav;
		}
	}
}

void PhysicsSystem::doPhysicsCollisions(float elapsed_ms) {

	auto& physics_registry = registry.physEntities;

	auto& motion_registry = registry.motions;

	float step_seconds = elapsed_ms / 1000.f;

	for (uint i = 0; i < physics_registry.size(); i++) {

		Physics& physComp = physics_registry.components[i];
		Entity entity = physics_registry.entities[i];

		physComp.velocity += (physComp.targetVelocity - physComp.velocity) * physComp.drag;

		Motion& motion = motion_registry.get(entity);
		motion.position += step_seconds * physComp.velocity;
	}
}

