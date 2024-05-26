#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createGameManager() {
	auto entity = Entity();

	registry.gameManagers.emplace(entity);
	registry.cameras.emplace(entity);

	return entity;
}

Entity createPlayer(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	// registry.meshPtrs.emplace(entity, &mesh);

	registry.inputs.emplace(entity);
	registry.physEntities.emplace(entity);
	registry.healths.emplace(entity);
	registry.gravEntities.emplace(entity);

	Collider& collider = registry.colliders.emplace(entity);
	collider.spr_scale = { 0.34f, 0.875f };
	collider.offset = { 0.f, 2.f };

	Mob& mob = registry.mobs.emplace(entity);
	mob.moveSpeed = 350.f;
	mob.jumpSpeed = 700.f;
	mob.knockbackSpeed = 300.f;

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.scale = { 32., 32. };

	registry.players.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity setAnimation(Entity e, TEXTURE_ASSET_ID sheet, uint numFrames, float frameRate) {
	// generate animation
	if (registry.animations.has(e)) registry.animations.remove(e);

	Animation& animation = registry.animations.emplace(e);
	animation.sheet = sheet;
	animation.numFrames = numFrames;
	animation.frameRate = frameRate;

	animation.vertex_buffers = std::vector<GLuint>(numFrames);

	glGenBuffers((GLsizei) numFrames, animation.vertex_buffers.data());
	glGenBuffers((GLsizei) 1, &(animation.indexBuffer));

	float texWidth = 1.f / numFrames;

	for (uint i = 0; i < numFrames; ++i) {
		float xCoord = (float) i / numFrames;
		std::vector<TexturedVertex> textured_vertices(4);
		textured_vertices[0].position = { -1.f/2, +1.f/2, 0.f };
		textured_vertices[1].position = { +1.f/2, +1.f/2, 0.f };
		textured_vertices[2].position = { +1.f/2, -1.f/2, 0.f };
		textured_vertices[3].position = { -1.f/2, -1.f/2, 0.f };

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
	if (registry.animations.has(e)) registry.animations.remove(e);
}

Entity createSolid(RenderSystem* renderer, vec2 pos, vec2 scale) {
	auto entity = Entity(); 

	registry.colliders.emplace(entity);
	registry.solids.emplace(entity);

	registry.physEntities.emplace(entity);


	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = scale;
	motion.angle = 0.f;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::SOLID, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

// TODO: ADD SPRITES OR TYPING
Entity createItem(RenderSystem* renderer, vec2 pos, vec2 im_scale, vec2 collider_scale, bool requiresInput) {
	Entity entity = Entity();

	registry.colliders.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.scale = im_scale;

	Interactable& i = registry.interactables.emplace(entity);
	i.boundsScale = collider_scale;
	i.requiresInput = requiresInput;

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::DEFAULT,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		}
	);

	return entity;

	return entity;
}