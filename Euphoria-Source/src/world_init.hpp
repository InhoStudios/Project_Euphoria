#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float BUG_BB_WIDTH = 300.f;
const float BUG_BB_HEIGHT = 300.f;
const float EAGLE_BB_WIDTH = 32.f;
const float EAGLE_BB_HEIGHT = 32.f;

const float TILE_SIZE = 16.f;

// the player
Entity createPlayer(RenderSystem* renderer, vec2 pos);
// the prey
Entity createBug(RenderSystem* renderer, vec2 position);
// the enemy
Entity createEagle(RenderSystem* renderer, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);
// a egg
Entity createEgg(vec2 pos, vec2 size);


