// internal
#include "animation_system.hpp"
#include <iostream>
#include "world_init.hpp"

void setAnimations() {

	for (int i = 0; i < registry.mobs.size(); i++) {

		Entity p_ent = registry.mobs.entities[i];

		Physics& physics = registry.physEntities.get(p_ent);
		//Animation& anim = registry.animations.get(p_ent);
		vec2 anim_threshold = { 30, 120 };

		if (!physics.inAir) {
			if (abs(physics.velocity.x) > anim_threshold.x) {
				setAnimation(p_ent, TEXTURE_ASSET_ID::PLAYER_RUN, 10, 0, 15);
			}
			else {
				setAnimation(p_ent, TEXTURE_ASSET_ID::PLAYER_IDLE, 14, 0, 12);
			}
		}
		else {
			if (physics.velocity.y > anim_threshold.y) {
				setAnimation(p_ent, TEXTURE_ASSET_ID::PLAYER_JUMP, 3, 2, 0);
			}
			else if (physics.velocity.y < -anim_threshold.y) {
				setAnimation(p_ent, TEXTURE_ASSET_ID::PLAYER_JUMP, 3, 0, 0);
			}
			else {
				setAnimation(p_ent, TEXTURE_ASSET_ID::PLAYER_JUMP, 3, 1, 0);
			}
		}
	}


}

void AnimationSystem::step(float elapsed_ms)
{
	setAnimations();
	(void)elapsed_ms; // placeholder to silence unused warning until implemented
	for (Animation& animation : registry.animations.components) {
		animation.msCounter += elapsed_ms;

		if (animation.frameRate == 0) continue;
		if (animation.msCounter >= (1000. / animation.frameRate)) {
			animation.index = (animation.index + 1) % animation.numFrames;
			animation.msCounter = 0.f;
		}
	}
}