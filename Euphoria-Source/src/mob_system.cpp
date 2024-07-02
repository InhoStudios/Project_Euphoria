#include "mob_system.hpp"

void MobSystem::step(float elapsed_ms) {
	Entity player = registry.players.entities[0];
	Player& playerComp = registry.players.components[0];
	Input& playerInput = registry.inputs.get(player);

	if (playerInput.key_release[KEY::ENHANCE]) {
		playerComp.enhanced = !playerComp.enhanced;
	}
	
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