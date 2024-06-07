// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include <iostream>

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

bool collides_at(Entity entity, Entity collidesWith, vec2 offsetPos) {

	Motion& entityMotion = registry.motions.get(entity);
	Motion& otherMotion = registry.motions.get(collidesWith);

	Collider& ec = registry.colliders.get(entity);
	Collider& oc = registry.colliders.get(collidesWith);

	vec2 m1_pos = entityMotion.position + ec.offset, 
		m2_pos = otherMotion.position + oc.offset;
	vec2 m1_bb = get_bounding_box(entityMotion), 
		m2_bb = get_bounding_box(otherMotion);

	// idea: for both bounding boxes, check if any corner is within any of the other bounding boxes
	int m1_minx = m1_pos[0] + offsetPos[0] - ec.spr_scale.x * m1_bb[0] / 2;
	int m1_miny = m1_pos[1] + offsetPos[1] - ec.spr_scale.y * m1_bb[1] / 2;
	int m1_maxx = m1_pos[0] + offsetPos[0] + ec.spr_scale.x * m1_bb[0] / 2;
	int m1_maxy = m1_pos[1] + offsetPos[1] + ec.spr_scale.y * m1_bb[1] / 2;

	int m2_minx = m2_pos[0] - oc.spr_scale.x * m2_bb[0] / 2;
	int m2_miny = m2_pos[1] - oc.spr_scale.y * m2_bb[1] / 2;
	int m2_maxx = m2_pos[0] + oc.spr_scale.x * m2_bb[0] / 2;
	int m2_maxy = m2_pos[1] + oc.spr_scale.y * m2_bb[1] / 2;

	return (
		m1_minx < m2_maxx &&
		m1_maxx > m2_minx &&
		m1_miny < m2_maxy &&
		m1_maxy > m2_miny
		);
}

void PhysicsSystem::checkCollisions() {
	ComponentContainer<Collider>& colliders = registry.colliders;
	for (uint i = 0; i < colliders.components.size(); i++)
	{
		Entity entity_i = colliders.entities[i];

		// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
		for (uint j = i + 1; j < colliders.components.size(); j++)
		{
			Entity entity_j = colliders.entities[j];
			if (collides_at(entity_i, entity_j))
			{
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
		}
	}
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move bug based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	doMobInput(elapsed_ms);
	doGravity(elapsed_ms);
	doPhysicsCollisions(elapsed_ms);

}

void doDashes(Entity& p, float elapsed_ms) {
	// Motion& mobMotion = registry.motions.get(p);
	Physics& physics = registry.physEntities.get(p);
	Mob& mob = registry.mobs.get(p);

	Input& input = registry.inputs.get(p);

	int hdir = input.key[KEY::RIGHT] - input.key[KEY::LEFT];

	// get key input dir

	if (registry.dashKits.has(p)) {
		DashKit& d = registry.dashKits.get(p);

		if (input.key_press[KEY::DASH]) {

			if ((mob.state != MOB_STATE::DASH && d.cd <= 0.1f) ||
				(d.enabled_dashes & CHAIN_DASH && d.cd < d.ctMax && d.cd > d.ctMin)) {
				if (!physics.inAir || d.enabled_dashes & MID_AIR_DASH) {
					debugging.dashDuration = 0.f;

					d.cd = d.cdTime;

					mob.state = MOB_STATE::DASH;
					physics.velocity.x = hdir * d.dashSpeed;
					physics.targetVelocity.x = 0;
				}
			}
		}

		if (mob.state == MOB_STATE::DASH) {
			if (physics.velocity.x < 250 && physics.velocity.x > -250) {
				mob.state = MOB_STATE::MOVE;
			}
		}

		// DEBUGGING
		if (debugging.in_debug_mode) {
			switch (mob.state) {
			case MOB_STATE::DASH:
				debugging.dashDuration += elapsed_ms;
				break;
			default:
				break;
			}
		}

		d.cd -= elapsed_ms;
		d.cd = fmax(0.f, d.cd);
	}
}

void doJumps(Entity& p) {
	Motion& mobMotion = registry.motions.get(p);
	Physics& physics = registry.physEntities.get(p);
	Mob& mob = registry.mobs.get(p);

	Input& input = registry.inputs.get(p);

	int hdir = input.key[KEY::RIGHT] - input.key[KEY::LEFT];

	if (input.key_press[KEY::JUMP]) {
		if (physics.onWall && mob.wallJumps < mob.maxWallJumps) {
			int facing = (mobMotion.scale.x > 0) - (mobMotion.scale.x < 0);

			physics.targetVelocity.x = -facing * mob.jumpSpeed;
			physics.velocity.x = physics.targetVelocity.x;

			physics.targetVelocity.y = -mob.jumpSpeed;
			physics.velocity.y = physics.targetVelocity.y;
			mob.wallJumps++;

			// air jumps
		}
		else if (!physics.inAir ||
			mob.coyoteMS < mob.maxCoyoteMS) {
			physics.targetVelocity.y = -mob.jumpSpeed;
			physics.velocity.y = physics.targetVelocity.y;
		}
		else if (mob.airJumps < mob.maxAirJumps) {
			physics.targetVelocity.y = -mob.jumpSpeed;
			physics.velocity.y = physics.targetVelocity.y;
			mob.airJumps++;
		}
	}

	if (input.key_release[KEY::JUMP] && physics.velocity.y < 0) {
		physics.targetVelocity.y = 0;
	}

}

void PhysicsSystem::doMobInput(float elapsed_ms) {
	// iterate through players
	auto& mob_registry = registry.mobs;
	for (uint i = 0; i < mob_registry.size(); i++) {
		Entity entity = mob_registry.entities[i];

		Motion& mobMotion = registry.motions.get(entity);
		Physics& physics = registry.physEntities.get(entity);
		Mob& mob = registry.mobs.get(entity);

		Input& input = registry.inputs.get(entity);

		int hdir = input.key[KEY::RIGHT] - input.key[KEY::LEFT];
		
		if (mob.state == MOB_STATE::MOVE) {
			physics.targetVelocity.x = hdir * mob.moveSpeed;
			doJumps(entity);
		}

		if (hdir) {
			mobMotion.scale.x = hdir * abs(mobMotion.scale.x);
		}

		doDashes(entity, elapsed_ms);
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

		// player check
		if (registry.mobs.has(entity) && registry.mobs.get(entity).state != MOB_STATE::MOVE) {
			continue;
		}

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
	auto& mob_registry = registry.mobs;
	auto& collider_registry = registry.colliders;

	float step_seconds = elapsed_ms / 1000.f;

	for (uint i = 0; i < physics_registry.size(); i++) {

		Physics& physComp = physics_registry.components[i];
		Entity entity = physics_registry.entities[i];

		physComp.velocity += (physComp.targetVelocity - physComp.velocity) * physComp.drag;

		Motion& motion = motion_registry.get(entity);

		vec2 startPos = motion.position;

		float hsp = step_seconds * (int) physComp.velocity.x, vsp = step_seconds * (int) physComp.velocity.y;
		int targHsp = step_seconds * physComp.targetVelocity.x;
		int shsp = (hsp > 0) - (hsp < 0), svsp = (vsp > 0) - (vsp < 0);

		physComp.inAir = true;
		physComp.onWall = false;
		
		if (registry.mobs.has(entity))
			registry.mobs.get(entity).checkedFrame = false;

		if (!registry.solids.has(entity) && registry.colliders.has(entity)) {
			for (uint j = 0; j < collider_registry.size(); j++) {
				Entity otherEntity = collider_registry.entities[j];
				Motion otherMotion = motion_registry.get(otherEntity);

				if (otherEntity == entity) continue;

				if (registry.solids.has(otherEntity)) {
					Solid& solid = registry.solids.get(otherEntity);

					// IN AIR CHECKS
					if (collides_at(entity, otherEntity, { 0., 2. }) && vsp >= 0) {
						physComp.inAir = false;
						if (registry.mobs.has(entity)) {
							// on ground: jump reset
							Mob& m = registry.mobs.get(entity);
							m.airJumps = 0;
							m.wallJumps = 0;
							m.coyoteMS = 0;
						}
					}
					else if (registry.mobs.has(entity)) {
						Mob& m = registry.mobs.get(entity);
						if (m.coyoteMS < m.maxCoyoteMS && !m.checkedFrame) {
							m.coyoteMS += elapsed_ms;
							m.checkedFrame = true;
						}
					}
					// ON WALL CHECKS
					if (registry.players.has(entity)) {
						// Player& p = registry.players.get(entity);
						Input& input = registry.inputs.get(entity);

						if ((
								collides_at(entity, otherEntity, { 1, 0 }) ||
								collides_at(entity, otherEntity, { -1, 0 })
							) &&
							input.key[KEY::GRAB] &&
							physComp.inAir) {
							if (vsp > 1) {
								vsp = 1;
								physComp.targetVelocity.y = (float)vsp / step_seconds;
								physComp.velocity.y = (float)vsp / step_seconds;
							}
							physComp.onWall = true;
						}
					}
					
					// COLLISION CODE
					if (collides_at(entity, otherEntity, { 0., vsp })) {
						motion.position = startPos;
						physComp.targetVelocity.y = -(physComp.elasticity * physComp.targetVelocity.y);
						physComp.velocity.y = -(physComp.elasticity * physComp.velocity.y);

						for (i = 0; i < 2 * abs(vsp); ++i) {
							if (collides_at(entity, otherEntity, { 0., svsp })) break;
							motion.position.y += svsp;
						}
						vsp = 0;
					}

					if (collides_at(entity, otherEntity, { hsp, 0. })) {
						int yplus = 0;

						while (collides_at(entity, otherEntity, { hsp, -yplus }) && 
							yplus <= (physComp.rampSpeed * TILE_SIZE / 2)) {
							yplus++;
						}

						if (collides_at(entity, otherEntity, { hsp, -yplus })) {
							motion.position.x = startPos.x;

							physComp.targetVelocity.x = -(physComp.elasticity * physComp.targetVelocity.x);
							physComp.velocity.x = -(physComp.elasticity * physComp.velocity.x);

							for (int i = 0; i < 2 * abs(hsp); i++) {
								if (collides_at(entity, otherEntity, { shsp, 0. })) break;
								motion.position.x += shsp;
							}
							hsp = 0;
						}
						else {
							motion.position.y -= yplus;
						}
					}

				}
			}
			motion.position += vec2({ hsp, vsp });
		}

	}
}

