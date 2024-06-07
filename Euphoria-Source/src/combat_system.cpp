#include "combat_system.hpp"

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
		float knockback, int damage, int ttl) {
	Entity entity;

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = scale;

	DamageCollider& dmg = registry.damageColliders.emplace(entity);
	dmg.source = source;
	dmg.ttl = ttl;

	return entity;
}

void CombatSystem::step() {
	for (uint i = 0; i < registry.collisions.size(); i++) {
		Entity eThis = registry.collisions.entities[i];
		Entity eOther = registry.collisions.components[i].other;

		if (registry.mobs.has(eThis) && registry.damageColliders.has(eOther) && registry.damageColliders.get(eOther).source != eThis) {
			DamageCollider& dc = registry.damageColliders.get(eOther);
			// take damage
			// do damage
		}
	}
}