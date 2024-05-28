#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float PLAYER_DIMS = 32.f;

Entity createGameManager();
// the player
Entity createPlayer(vec2 pos);

// enemy
Entity createEnemy(vec2 pos);

Entity setAnimation(Entity e, TEXTURE_ASSET_ID sheet, uint numFrames, float frameRate);
void clearAnimation(Entity e);

Entity createSolid(vec2 pos, vec2 scale);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);

Entity createItem(vec2 pos, vec2 im_scale, vec2 collider_scale, bool requiresInput);

Entity createTransition(TransitionData& t);
