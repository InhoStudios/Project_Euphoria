#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"


class AnimationSystem
{
public:
	AnimationSystem() {
		
	}
	void step(float elapsed_ms);
};