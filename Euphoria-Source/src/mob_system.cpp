#include "mob_system.hpp"

void MobSystem::step(float elapsed_ms) {
	
	for (int i = 0; i < registry.mobs.size(); ++i) {
		Mob& mob = registry.mobs.components[i];

		if (mob.stateTimer <= 0.f) {
			// timer behaviour
			if (mob.state == MOB_STATE::KNOCKBACK) {
				mob.state = MOB_STATE::MOVE;
			}
		}

		if (mob.stateTimer > 0.f) mob.stateTimer -= elapsed_ms;
	}

}