#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

class MobSystem
{
public:
	MobSystem() { }
	void step(float elapsed_ms);
private:
};