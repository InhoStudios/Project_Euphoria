// internal
#include "animation_system.hpp"
#include <iostream>

void AnimationSystem::step(float elapsed_ms)
{
	(void)elapsed_ms; // placeholder to silence unused warning until implemented
	for (Animation& animation : registry.animations.components) {
		animation.msCounter += elapsed_ms;

		if (animation.frameRate <= 0) continue;
		if (animation.msCounter >= (1000. / animation.frameRate)) {
			animation.index = (animation.index + 1) % animation.numFrames;
			animation.msCounter = 0.f;
		}
	}
}