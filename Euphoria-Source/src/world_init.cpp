#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity setAnimation(Entity e, TEXTURE_ASSET_ID sheet, int numFrames, int index, float frameRate) {
	// generate animation
	if (registry.animations.has(e)) {
		if (registry.animations.get(e).sheet != sheet) {
			registry.animations.remove(e);
		}
		else {
			Animation& animation = registry.animations.get(e);
			if (index != 0 || frameRate == 0) {
				animation.index = index;
			}
			animation.frameRate = frameRate;
			return e;
		}
	}

	Animation& animation = registry.animations.emplace(e);
	animation.sheet = sheet;
	animation.numFrames = numFrames;
	animation.index = index;
	animation.frameRate = frameRate;

	animation.vertex_buffers = std::vector<GLuint>(numFrames);

	glGenBuffers((GLsizei)numFrames, animation.vertex_buffers.data());
	glGenBuffers((GLsizei)1, &(animation.indexBuffer));

	float texWidth = 1.f / numFrames;

	for (uint i = 0; i < numFrames; ++i) {
		float xCoord = (float)i / numFrames;
		std::vector<TexturedVertex> textured_vertices(4);
		textured_vertices[0].position = { -1.f / 2, +1.f / 2, 0.f };
		textured_vertices[1].position = { +1.f / 2, +1.f / 2, 0.f };
		textured_vertices[2].position = { +1.f / 2, -1.f / 2, 0.f };
		textured_vertices[3].position = { -1.f / 2, -1.f / 2, 0.f };

		textured_vertices[0].texcoord = { xCoord, 1.f };
		textured_vertices[1].texcoord = { xCoord + texWidth, 1.f };
		textured_vertices[2].texcoord = { xCoord + texWidth, 0.f };
		textured_vertices[3].texcoord = { xCoord, 0.f };

		glBindBuffer(GL_ARRAY_BUFFER, animation.vertex_buffers[i]);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(textured_vertices[0]) * textured_vertices.size(), textured_vertices.data(), GL_STATIC_DRAW);
		gl_has_errors();
	}
	// Counterclockwise as it's the default opengl front winding direction.
	const std::vector<uint16_t> textured_indices = { 0, 3, 1, 1, 3, 2 };

	glBindBuffer(GL_ARRAY_BUFFER, animation.indexBuffer);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(textured_indices[0]) * textured_indices.size(), textured_indices.data(), GL_STATIC_DRAW);
	gl_has_errors();

	return e;
}

void clearAnimation(Entity e) {
	if (registry.animations.has(e)) {
		Animation& a = registry.animations.get(e);
		// TODO: CLEAR AND FREE BUFFER DATA BEFORE REMOVING THE COMPONENT HERE
		glDeleteBuffers((GLsizei)a.vertex_buffers.size(), a.vertex_buffers.data());
		glDeleteBuffers((GLsizei)1, &a.indexBuffer);
		registry.animations.remove(e);
	}
}

Entity createGameManager() {
	auto entity = Entity();

	registry.gameManagers.emplace(entity);
	registry.cameras.emplace(entity);

	return entity;
}

Entity createPlayer(vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	// registry.meshPtrs.emplace(entity, &mesh);

	registry.players.emplace(entity);
	registry.inputs.emplace(entity);

	DashKit& d = registry.dashKits.emplace(entity);
	d.enabled_dashes = d.enabled_dashes | MID_AIR_DASH;
	d.enabled_dashes = d.enabled_dashes | CHAIN_DASH;
	d.enabled_dashes = d.enabled_dashes | PARRY_DASH;

	registry.physEntities.emplace(entity);
	registry.healths.emplace(entity);
	registry.gravEntities.emplace(entity);

	Collider& collider = registry.colliders.emplace(entity);
	collider.spr_scale = { 0.375f, 0.875f };
	collider.offset = { 0.f, 2.f };

	Mob& mob = registry.mobs.emplace(entity);
	mob.equipped_atk = WEAPON_ID::CROWBAR;
	mob.moveSpeed = 240.f;
	mob.jumpSpeed = 480.f;
	mob.knockbackSpeed = 300.f;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.scale = { PLAYER_DIMS, PLAYER_DIMS };

	motion.visible = true;
	motion.used_texture = TEXTURE_ASSET_ID::PLAYER;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	motion.render_layer = RENDER_LAYER::ENTITIES;

	return entity;
}

Entity createEnemy(vec2 pos) {
	auto entity = Entity();

	registry.inputs.emplace(entity);

	DashKit& d = registry.dashKits.emplace(entity);

	registry.physEntities.emplace(entity);

	Health& h = registry.healths.emplace(entity);
	h.hp = 5;

	registry.gravEntities.emplace(entity);

	Collider& collider = registry.colliders.emplace(entity);
	collider.spr_scale = { 0.375f, 0.875f };
	collider.offset = { 0.f, 2.f };

	Mob& mob = registry.mobs.emplace(entity);
	mob.equipped_atk = WEAPON_ID::NO_WEAPON;
	mob.moveSpeed = 75.f;
	mob.jumpSpeed = 600.f;
	mob.knockbackSpeed = 200.f;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.scale = { PLAYER_DIMS, PLAYER_DIMS };

	motion.visible = true;
	motion.used_texture = TEXTURE_ASSET_ID::GB_ENEMY;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	motion.render_layer = RENDER_LAYER::ENTITIES;

	registry.levelElements.emplace(entity);
	return entity;
}

Entity createSolid(vec2 pos, vec2 scale, TEXTURE_ASSET_ID sprite) {
	auto entity = Entity(); 

	registry.colliders.emplace(entity);
	registry.solids.emplace(entity);

	registry.physEntities.emplace(entity);


	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = scale;
	motion.angle = 0.f;

	motion.visible = true;
	motion.used_texture = sprite;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	motion.render_layer = RENDER_LAYER::BG_DECOR;

	registry.levelElements.emplace(entity);
	return entity;
}

Entity createTiledSolid(vec2 pos, vec2 scale, TEXTURE_ASSET_ID sprite, uint index) {
	Entity entity = createSolid(pos, scale, sprite);

	setAnimation(entity, sprite, 47, index, 0.f);

	if ((index >= 8 && index <= 18) || index == 23 || (index >= 43 && index <= 46)) {
		registry.solids.remove(entity);
		registry.colliders.remove(entity);
		registry.physEntities.remove(entity);
	}

	return entity;
}

Entity createBackground(BackgroundData bg) {
	Entity entity;

	registry.backgrounds.insert(
		entity,
		{ bg.parallaxDistance,
			bg.pos }
	);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = bg.pos;
	motion.scale = bg.scale;

	motion.visible = true;

	motion.used_texture = bg.background;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	motion.render_layer = RENDER_LAYER::BACKGROUND;

	registry.levelElements.emplace(entity);
	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = position;
	motion.scale = scale;


	motion.visible = true;
	motion.used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::DEBUG_LINE;

	motion.render_layer = RENDER_LAYER::DEBUG;

	registry.debugComponents.emplace(entity);
	return entity;
}

// TODO: ADD SPRITES OR TYPING
Entity createItem(vec2 pos, vec2 im_scale, vec2 collider_scale, 
	bool needsInput, TEXTURE_ASSET_ID sprite, ITEM_ID item) {
	Entity entity = Entity();

	registry.colliders.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = im_scale;

	Interactable& i = registry.interactables.emplace(entity);
	i.boundsScale = collider_scale;
	i.needsInput = needsInput;

	motion.visible = true;
	motion.used_texture = sprite;
	motion.used_effect = EFFECT_ASSET_ID::TEXTURED;
	motion.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	motion.render_layer = RENDER_LAYER::ITEMS;

	registry.items.insert(entity, { item });

	registry.levelElements.emplace(entity);
	return entity;
}

Entity createTransition(TransitionData& t) {
	Entity entity = Entity();

	registry.colliders.emplace(entity);

	Motion& m = registry.motions.emplace(entity);
	m.position = t.position;
	m.scale = t.scale;

	Interactable& i = registry.interactables.emplace(entity);
	i.needsInput = t.needsInput;

	registry.transitions.insert(
		entity,
		{
			t.targetLevel,
			t.targetPosition
		}
	);

	m.visible = true;
	m.used_texture = t.sprite;
	m.used_effect = EFFECT_ASSET_ID::TEXTURED;
	m.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;

	m.render_layer = RENDER_LAYER::BG_ELEMENTS;

	registry.levelElements.emplace(entity);
	return entity;
}