#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include "../ext/stb_image/stb_image.h"

// Player component
struct Player
{

};

struct Mob {

};

struct Physics {

};

struct Collider {

};

struct Solid {

};

// Eagles have a hard shell
struct Deadly
{

};

// Bug and Chicken have a soft shell
struct Eatable
{

};

enum class INPUT_KEY {
	KEY_RIGHT = 0,
	KEY_LEFT = KEY_RIGHT + 1,
	KEY_UP = KEY_LEFT + 1,
	KEY_DOWN = KEY_UP + 1,

	KEY_JUMP = KEY_DOWN + 1,

	KEY_A1 = KEY_JUMP + 1, // i or z
	KEY_A2 = KEY_UP + 1, // j or x
	KEY_A3 = KEY_UP + 1, // k or c
	KEY_A4 = KEY_UP + 1, // l or v
	KEY_MOD = KEY_UP + 1, // shift or enter
	
};

const std::map<INPUT_KEY, int> WASD_MAPPING = {
		{INPUT_KEY::KEY_RIGHT, GLFW_KEY_D},
		{INPUT_KEY::KEY_LEFT, GLFW_KEY_A},
		{INPUT_KEY::KEY_UP, GLFW_KEY_W},
		{INPUT_KEY::KEY_DOWN, GLFW_KEY_S},

		{INPUT_KEY::KEY_JUMP, GLFW_KEY_SPACE},

		{INPUT_KEY::KEY_A1, GLFW_KEY_I},
		{INPUT_KEY::KEY_A2, GLFW_KEY_J},
		{INPUT_KEY::KEY_A3, GLFW_KEY_K},
		{INPUT_KEY::KEY_A4, GLFW_KEY_L},
		{INPUT_KEY::KEY_MOD, GLFW_KEY_ENTER}
};

const std::map<INPUT_KEY, int> ARROW_MAPPING = {
		{INPUT_KEY::KEY_RIGHT, GLFW_KEY_RIGHT},
		{INPUT_KEY::KEY_LEFT, GLFW_KEY_LEFT},
		{INPUT_KEY::KEY_UP, GLFW_KEY_UP},
		{INPUT_KEY::KEY_DOWN, GLFW_KEY_DOWN},

		{INPUT_KEY::KEY_JUMP, GLFW_KEY_SPACE},

		{INPUT_KEY::KEY_A1, GLFW_KEY_Z},
		{INPUT_KEY::KEY_A2, GLFW_KEY_X},
		{INPUT_KEY::KEY_A3, GLFW_KEY_C},
		{INPUT_KEY::KEY_A4, GLFW_KEY_V},
		{INPUT_KEY::KEY_MOD, GLFW_KEY_LEFT_SHIFT}
};

struct Input {
	// map of input_key to GLFW keys
	std::map<INPUT_KEY, int> key_mapping = WASD_MAPPING;
	std::map<INPUT_KEY, bool> key = {
		{INPUT_KEY::KEY_RIGHT, false},
		{INPUT_KEY::KEY_LEFT, false},
		{INPUT_KEY::KEY_UP, false},
		{INPUT_KEY::KEY_DOWN, false},

		{INPUT_KEY::KEY_JUMP, false},

		{INPUT_KEY::KEY_A1, false},
		{INPUT_KEY::KEY_A2, false},
		{INPUT_KEY::KEY_A3, false},
		{INPUT_KEY::KEY_A4, false},
		{INPUT_KEY::KEY_MOD, false},
	};

	std::map<INPUT_KEY, bool> key_press = {
		{INPUT_KEY::KEY_RIGHT, false},
		{INPUT_KEY::KEY_LEFT, false},
		{INPUT_KEY::KEY_UP, false},
		{INPUT_KEY::KEY_DOWN, false},

		{INPUT_KEY::KEY_JUMP, false},

		{INPUT_KEY::KEY_A1, false},
		{INPUT_KEY::KEY_A2, false},
		{INPUT_KEY::KEY_A3, false},
		{INPUT_KEY::KEY_A4, false},
		{INPUT_KEY::KEY_MOD, false},
	};

	std::map<INPUT_KEY, bool> key_release = {
		{INPUT_KEY::KEY_RIGHT, false},
		{INPUT_KEY::KEY_LEFT, false},
		{INPUT_KEY::KEY_UP, false},
		{INPUT_KEY::KEY_DOWN, false},

		{INPUT_KEY::KEY_JUMP, false},

		{INPUT_KEY::KEY_A1, false},
		{INPUT_KEY::KEY_A2, false},
		{INPUT_KEY::KEY_A3, false},
		{INPUT_KEY::KEY_A4, false},
		{INPUT_KEY::KEY_MOD, false},
	};
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

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
	BUG = 0,
	EAGLE = BUG + 1,
	PLAYER = EAGLE + 1,
	TEXTURE_COUNT = PLAYER + 1
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

