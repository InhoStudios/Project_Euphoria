// internal
#include "animation_system.hpp"
#include <iostream>
#include "world_init.hpp"

void setAnimations() {

	for (int i = 0; i < registry.mobs.size(); i++) {

		Entity p_ent = registry.mobs.entities[i];
		Mob& p_mob = registry.mobs.components[i];

		Physics& physics = registry.physEntities.get(p_ent);
		Motion& m = registry.motions.get(p_ent);
		//Animation& anim = registry.animations.get(p_ent);
		vec2 anim_threshold = { 30, 120 };

		int hdir = (m.scale.x > 0) - (m.scale.x < 0);

		TEXTURE_ASSET_ID idle, run, jump;

		switch (p_mob.equipped_atk) {
		case WEAPON_ID::CROWBAR:
			idle = TEXTURE_ASSET_ID::PLAYER_IDLE_CROWBAR;
			run = TEXTURE_ASSET_ID::PLAYER_RUN_CROWBAR;
			jump = TEXTURE_ASSET_ID::PLAYER_JUMP_CROWBAR;
			break;
		default:
			idle = TEXTURE_ASSET_ID::PLAYER_IDLE;
			run = TEXTURE_ASSET_ID::PLAYER_RUN;
			jump = TEXTURE_ASSET_ID::PLAYER_JUMP;
			break;
		}

		switch (p_mob.state) {
		case MOB_STATE::MOVE:
			if (!physics.inAir) {
				if (abs(physics.velocity.x) > anim_threshold.x) {
					setAnimation(p_ent, run, 10, 0, 15);
				}
				else {
					setAnimation(p_ent, idle, 14, 0, 12);
				}
			}
			else {
				if (physics.velocity.y > anim_threshold.y) {
					setAnimation(p_ent, jump, 3, 2, 0);
				}
				else if (physics.velocity.y < -anim_threshold.y) {
					setAnimation(p_ent, jump, 3, 0, 0);
				}
				else {
					setAnimation(p_ent, jump, 3, 1, 0);
				}
			}
			break;
		case MOB_STATE::ATTACK:
			Animation& a = registry.animations.get(p_ent);
			if (a.index == a.numFrames - 1) {
				p_mob.state = MOB_STATE::MOVE;
				m.scale.x = hdir * PLAYER_DIMS;
			}
			break;
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