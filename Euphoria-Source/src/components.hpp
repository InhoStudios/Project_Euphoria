#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include "../ext/stb_image/stb_image.h"

enum class GAME_STATE {
	MENU = 0,
	PLAY = MENU + 1,
	PAUSE = PLAY + 1,

	//
};

struct GameManager {

};

// Player component
struct Player
{
	// put enhancements in player component? or in game manager?
	uint coyoteMS = 0;
	uint maxCoyoteMS = 60;
	uint airJumps = 0;
	uint maxAirJumps = 0;
	uint wallJumps = 0;
	uint maxWallJumps = 3;
	bool checkedFrame = false;
};

struct Mob {
	float moveSpeed;
	float jumpSpeed;
	float knockbackSpeed;
	int numJumps;
};

struct Health {
	int hp = 10;
};

struct Physics {
	vec2 velocity = { 0.f, 0.f };
	vec2 targetVelocity = { 0.f, 0.f };
	bool inAir = false;
	bool onWall = false;
	float elasticity = 0.0;
	float drag = 0.15;
	float rampSpeed = 1.0;
};

struct Gravity {
	float grav = 100.f * TILE_SIZE;
	float terminalVelocity = 1000.f;
};

struct Collider {
	std::vector<vec2> hull;
};

struct Solid {

};

struct Camera {
	vec2 targetPosition = { 0.f, 0.f };
	vec2 position = { 0.f, 0.f };
	vec2 dims = { 640., 360. };

	float defaultZoom = 1.0;
	float zoom = defaultZoom;
	float interpSpeed = 0.3;
	vec2 offset = { 16.f, 64.f };

	vec2 bounds; // dynamically set
};

enum class KEY {
	RIGHT = 0,
	LEFT = RIGHT + 1,
	UP = LEFT + 1,
	DOWN = UP + 1,

	JUMP = DOWN + 1,

	BASIC = JUMP + 1, // i or z
	SPECIAL = BASIC + 1, // j or x
	GRAB = SPECIAL + 1, // k or c
	ENHANCE = GRAB + 1, // l or v
	DASH = ENHANCE + 1, // shift
	
};

const std::map<KEY, int> WASD_MAPPING = {
		{KEY::RIGHT, GLFW_KEY_D},
		{KEY::LEFT, GLFW_KEY_A},
		{KEY::UP, GLFW_KEY_W},
		{KEY::DOWN, GLFW_KEY_S},

		{KEY::JUMP, GLFW_KEY_SPACE},

		{KEY::BASIC, GLFW_KEY_J},
		{KEY::SPECIAL, GLFW_KEY_K},
		{KEY::GRAB, GLFW_KEY_I},
		{KEY::ENHANCE, GLFW_KEY_L},
		{KEY::DASH, GLFW_KEY_LEFT_SHIFT}
};

const std::map<KEY, int> ARROW_MAPPING = {
		{KEY::RIGHT, GLFW_KEY_RIGHT},
		{KEY::LEFT, GLFW_KEY_LEFT},
		{KEY::UP, GLFW_KEY_UP},
		{KEY::DOWN, GLFW_KEY_DOWN},

		{KEY::JUMP, GLFW_KEY_SPACE},

		{KEY::BASIC, GLFW_KEY_Z},
		{KEY::SPECIAL, GLFW_KEY_X},
		{KEY::GRAB, GLFW_KEY_C},
		{KEY::ENHANCE, GLFW_KEY_V},
		{KEY::DASH, GLFW_KEY_LEFT_SHIFT}
};

struct Input {
	// map of KEY to GLFW keys
	std::map<KEY, int> key_mapping = WASD_MAPPING;
	std::map<KEY, bool> key = {
		{KEY::RIGHT, false},
		{KEY::LEFT, false},
		{KEY::UP, false},
		{KEY::DOWN, false},

		{KEY::JUMP, false},

		{KEY::BASIC, false},
		{KEY::SPECIAL, false},
		{KEY::GRAB, false},
		{KEY::ENHANCE, false},
		{KEY::DASH, false},
	};

	std::map<KEY, bool> key_press = {
		{KEY::RIGHT, false},
		{KEY::LEFT, false},
		{KEY::UP, false},
		{KEY::DOWN, false},

		{KEY::JUMP, false},

		{KEY::BASIC, false},
		{KEY::SPECIAL, false},
		{KEY::GRAB, false},
		{KEY::ENHANCE, false},
		{KEY::DASH, false},
	};

	std::map<KEY, bool> key_release = {
		{KEY::RIGHT, false},
		{KEY::LEFT, false},
		{KEY::UP, false},
		{KEY::DOWN, false},

		{KEY::JUMP, false},

		{KEY::BASIC, false},
		{KEY::SPECIAL, false},
		{KEY::GRAB, false},
		{KEY::ENHANCE, false},
		{KEY::DASH, false},
	};
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 scale = { 10, 10 };
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// LEVEL COMPONENTS

struct LevelElement {

};

// END LEVEL COMPONENTS

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// Sets the brightness of the screen
struct ScreenState
{
	float darken_screen_factor = -1;
};

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// A timer that will be associated to dying chicken
struct DeathTimer
{
	float counter_ms = 3000;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & chicken.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID {
	PLAYER = 0,
	SOLID = PLAYER + 1,
	TEST_ANIM = SOLID + 1,
	TEXTURE_COUNT = TEST_ANIM + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	EGG = COLOURED + 1,
	CHICKEN = EGG + 1,
	TEXTURED = CHICKEN + 1,
	WIND = TEXTURED + 1,
	EFFECT_COUNT = WIND + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	CHICKEN = 0,
	SPRITE = CHICKEN + 1,
	EGG = SPRITE + 1,
	DEBUG_LINE = EGG + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	GEOMETRY_COUNT = SCREEN_TRIANGLE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

struct Animation
{
	TEXTURE_ASSET_ID sheet;
	uint numFrames;
	uint index;
	float frameRate;
	float msCounter = 0.f;

	std::vector<GLuint> vertex_buffers;
	GLuint indexBuffer; // we only need one index buffer for each frame i think?
};