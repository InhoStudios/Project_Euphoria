// internal
#include "ai_system.hpp"

void AISystem::step(float elapsed_ms)
{
	(void)elapsed_ms; // placeholder to silence unused warning until implemented

	auto& mob_registry = registry.mobs;
	for (uint i = 0; i < mob_registry.size(); i++) {
		Entity& e = mob_registry.entities[i];
		Mob& mob = mob_registry.components[i];

		if (mob.state != MOB_STATE::MOVE) continue;

		Motion& mm = registry.motions.get(e);
		Physics& mp = registry.physEntities.get(e);
		Input& mi = registry.inputs.get(e);

		if (registry.players.has(e)) continue;

		if ((mi.key[KEY::LEFT] || mi.key[KEY::RIGHT]) && abs(mp.velocity.x) <= 0.01 && !mp.inAir) {
			mi.key_press[KEY::JUMP] = true;
		} else if (mp.inAir && abs(mp.velocity.x) >= 0.01) {
			mi.key_release[KEY::JUMP] = true;
		}

		Motion& pm = registry.motions.get(registry.players.entities[0]);

		float diff = pm.position.x - mm.position.x;

		mi.key[KEY::LEFT] = abs(diff) > 32 && (diff < 0);
		mi.key[KEY::RIGHT] = abs(diff) > 32 && (diff > 0);
	}
}