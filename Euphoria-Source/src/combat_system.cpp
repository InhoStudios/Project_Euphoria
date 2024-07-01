#include "combat_system.hpp"

WeaponRegistry weapon;

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
		float knockback, int damage, int ttl) {
	Entity entity;

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = scale;

	motion.visible = true;
	motion.used_texture = TEXTURE_ASSET_ID::HITBOX;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	registry.colliders.emplace(entity);

	DamageCollider& dmg = registry.damageColliders.emplace(entity);
	dmg.source = source;
	dmg.knockback = { knockback, - 0.5f * abs(knockback)};
	dmg.dmg = damage;
	dmg.ttl = ttl;

	registry.levelElements.emplace(entity);

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

		if (mob.equipped_atk == WEAPON_ID::NO_WEAPON) continue;
		Weapon* wpn = weapon.weapons[(int)mob.equipped_atk];
		POINT_DIRS dir = getInputPointingDirection(input, motion, wpn->basic_dirl);

		if (input.key_press[KEY::BASIC]) {
			Entity dmgEnt;

			// do jolt dir
			float angle = ((int)dir - 4) * (M_PI / 4);
			float x = cos(angle);
			float y = sin(angle);

			phys.velocity.x = x * wpn->basic_jolt;
			phys.targetVelocity.x = x * wpn->basic_jolt;

			phys.velocity.y = y * wpn->basic_jolt;
			phys.targetVelocity.y = y * wpn->basic_jolt;
			mob.state = MOB_STATE::KNOCKBACK;
			mob.stateTimer = wpn->basic_jolt_time;

			switch (mob.equipped_atk) {
			case WEAPON_ID::CROWBAR:
				dmgEnt = instantiateDamage(entity, motion.position + vec2({ facing * wpn->basic_range, 0.f }),
					{ 2 * TILE_SIZE, TILE_SIZE }, facing * wpn->basic_kb, wpn->basic_dmg, 25);
				break;
			case WEAPON_ID::SHOTGUN:
			{
				dmgEnt = instantiateDamage(entity, motion.position + vec2({ 0.f, 0.f }),
					{ 1.5 * TILE_SIZE, 1.5 * TILE_SIZE }, facing * wpn->basic_kb, wpn->basic_dmg, 250);
				Physics& projectile = registry.physEntities.emplace(dmgEnt);
				projectile.velocity = { 1600 * x, 1600 * y };
				projectile.targetVelocity = { 100 * x, 100 * y };
				break;
			}
			case WEAPON_ID::BASEBALL_BAT:
				break;
			case WEAPON_ID::BRASS_KNUCKLES:
				break;
			case WEAPON_ID::RAILGUN:
				break;
			case WEAPON_ID::POP_SICKLE:
				break;
			case WEAPON_ID::ARM_BLADES:
				break;
			}

		}
		else if (input.key_press[KEY::SPECIAL]) {
			Entity dmgEnt;

			// do jolt dir
			float angle = ((int)dir - 4) * (M_PI / 4);
			float x = cos(angle);
			float y = sin(angle);

			phys.velocity.x = x * wpn->special_jolt;
			phys.targetVelocity.x = x * wpn->special_jolt;
			phys.velocity.y = y * wpn->special_jolt;
			phys.targetVelocity.y = y * wpn->special_jolt;
			mob.state = MOB_STATE::KNOCKBACK;
			mob.stateTimer = wpn->special_jolt_time;

			switch (mob.equipped_atk) {
			case WEAPON_ID::CROWBAR:
				dmgEnt = instantiateDamage(entity, motion.position + vec2({ facing * wpn->special_range, 0.f }),
					{ 2 * TILE_SIZE, TILE_SIZE }, facing * wpn->special_kb, wpn->special_dmg, 25);
				break;
			case WEAPON_ID::SHOTGUN:
			{
				dmgEnt = instantiateDamage(entity, motion.position + vec2({ 0.f, 0.f }),
					{ 1.5 * TILE_SIZE, 1.5 * TILE_SIZE }, facing * wpn->special_kb, wpn->special_dmg, 250);
				Physics& projectile = registry.physEntities.emplace(dmgEnt);
				projectile.velocity = { 2000 * x, 2000 * y };
				projectile.targetVelocity = { 100 * x, 100 * y };
				break;
			}
			case WEAPON_ID::BASEBALL_BAT:
				break;
			case WEAPON_ID::BRASS_KNUCKLES:
				break;
			case WEAPON_ID::RAILGUN:
				break;
			case WEAPON_ID::POP_SICKLE:
				break;
			case WEAPON_ID::ARM_BLADES:
				break;
			}

		}
	}

	// COLLISIONS
	for (uint i = 0; i < registry.collisions.size(); ++i) {
		Entity eThis = registry.collisions.entities[i];
		Entity eOther = registry.collisions.components[i].other;

		if (registry.healths.has(eThis) && registry.damageColliders.has(eOther) && registry.damageColliders.get(eOther).source != eThis) {
			DamageCollider& dc = registry.damageColliders.get(eOther);
			// take damage
			Health& health = registry.healths.get(eThis);
			health.hp -= dc.dmg;
			if (registry.mobs.has(eThis)) {
				Mob& mob = registry.mobs.get(eThis);
				Input& i = registry.inputs.get(eThis);
				Physics& phys = registry.physEntities.get(eThis);

				i.key = {
					{KEY::RIGHT, false},
					{KEY::LEFT, false},
					{KEY::UP, false},
					{KEY::DOWN, false},

					{KEY::JUMP, false},

					{KEY::BASIC, false},
					{KEY::SPECIAL, false},
					{KEY::GRAB, false},
					{KEY::ENHANCE, false},
					{KEY::DASH, false},
				};
				i.key_press = {
					{KEY::RIGHT, false},
					{KEY::LEFT, false},
					{KEY::UP, false},
					{KEY::DOWN, false},

					{KEY::JUMP, false},

					{KEY::BASIC, false},
					{KEY::SPECIAL, false},
					{KEY::GRAB, false},
					{KEY::ENHANCE, false},
					{KEY::DASH, false},
				};
				i.key_release = {
					{KEY::RIGHT, false},
					{KEY::LEFT, false},
					{KEY::UP, false},
					{KEY::DOWN, false},

					{KEY::JUMP, false},

					{KEY::BASIC, false},
					{KEY::SPECIAL, false},
					{KEY::GRAB, false},
					{KEY::ENHANCE, false},
					{KEY::DASH, false},
				};

				phys.velocity = mob.knockbackSpeed * dc.knockback;
				phys.targetVelocity = 0.5f * mob.knockbackSpeed * dc.knockback;
				mob.state = MOB_STATE::KNOCKBACK;
				mob.stateTimer = 130.f;
			}

		}
	}
}