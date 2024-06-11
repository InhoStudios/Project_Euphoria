#include "combat_system.hpp"

WeaponRegistry weapon;

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
		float knockback, int damage, int ttl) {
	Entity entity;

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = scale;

	registry.colliders.emplace(entity);

	DamageCollider& dmg = registry.damageColliders.emplace(entity);
	dmg.source = source;
	dmg.knockback = { knockback, -abs(knockback) };
	dmg.dmg = damage;
	dmg.ttl = ttl;

	registry.levelElements.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HITBOX, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

void CombatSystem::step(float elapsed_ms) {
	// CLEAR COLLIDERS
	for (uint c = 0; c < registry.damageColliders.size(); ++c) {
		Entity entity = registry.damageColliders.entities[c];
		DamageCollider& dc = registry.damageColliders.components[c];

		dc.ttl -= elapsed_ms;

		if (dc.ttl < 0) {
			registry.remove_all_components_of(entity);
		}
	}

	// CREATE DAMAGE COLLIDERS
	for (uint i = 0; i < registry.mobs.size(); ++i) {
		Entity entity = registry.mobs.entities[i];
		Mob& mob = registry.mobs.components[i];
		Input& input = registry.inputs.get(entity);
		Motion& motion = registry.motions.get(entity);
		Physics& phys = registry.physEntities.get(entity);

		int facing = (motion.scale.x > 0) - (motion.scale.x < 0);

		if (input.key_press[KEY::BASIC]) {
			Weapon *wpn = weapon.weapons[(int) mob.equipped_atk];
			instantiateDamage(entity, motion.position + vec2({ facing * wpn->basic_range, 0.f }), 
				{2 * TILE_SIZE, TILE_SIZE}, facing * wpn->basic_kb, wpn->basic_dmg, 25);
			phys.velocity.x = facing * wpn->basic_jolt;
		}
	}

	// COLLISIONS
	for (uint i = 0; i < registry.collisions.size(); ++i) {
		Entity eThis = registry.collisions.entities[i];
		Entity eOther = registry.collisions.components[i].other;

		if (registry.mobs.has(eThis) && registry.damageColliders.has(eOther) && registry.damageColliders.get(eOther).source != eThis) {
			DamageCollider& dc = registry.damageColliders.get(eOther);
			// take damage
			Health& health = registry.healths.get(eThis);
			Mob& mob = registry.mobs.get(eThis);
			Physics& phys = registry.physEntities.get(eThis);

			health.hp -= dc.dmg;
			phys.velocity = mob.knockbackSpeed * dc.knockback;
			phys.targetVelocity = mob.knockbackSpeed * dc.knockback;
			mob.state = MOB_STATE::KNOCKBACK;

			registry.remove_all_components_of(eOther);
		}
	}
}