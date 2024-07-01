#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_map>
#include <map>
#include "../ext/stb_image/stb_image.h"

enum class GAME_STATE {
	MENU = 0,
	PLAY,
	PAUSE,
	FRAME_FREEZE,
	//
};

enum class LEVEL;
enum class TEXTURE_ASSET_ID;
enum class WEAPON_ID;
enum class MOVE_KIT_ID;

enum class ITEM_ID {
	DEFAULT,
	CROWBAR,
	BASEBALL_BAT,
	ITEM_COUNT
};
const int item_count = (int)ITEM_ID::ITEM_COUNT;

struct Motion;

struct GameManager {
	GAME_STATE current_state;
	LEVEL current_level;

	int inventory[item_count] = { 0 };

	vec2 bounds; // dynamically set 
};

enum class MOB_STATE {
	MOVE,
	ATTACK,
	DASH,
	KNOCKBACK,
	FLEE,
	INVINCIBLE
};

enum class POINT_DIRS {
	NEUTRAL,
	UP_LEFT,
	UP,
	UP_RIGHT,
	RIGHT,
	DOWN_RIGHT,
	DOWN,
	DOWN_LEFT,
	LEFT
};

// Player component
struct Player
{
	// put enhancements in player component? or in game manager?
	MOVE_KIT_ID equipped_MK;
};

#define MID_AIR_DASH  0b00000001
#define CHAIN_DASH    0b00000010
#define PARRY_DASH    0b00000100
#define SLASH_DASH    0b00001000
#define INVINCIBLE    0b00010000

struct DashKit
{
	int enabled_dashes = 0b00000000;
	float dashSpeed = 2000.f;
	float cd, cdTime = 600.f;
	float ctMax = 350.f, ctMin = 250.f;
};

struct Mob {
	// put enhancements in player component? or in game manager?
	MOB_STATE state = MOB_STATE::MOVE;
	uint coyoteMS = 0;
	uint maxCoyoteMS = 60;
	uint airJumps = 0;
	uint maxAirJumps = 0;
	uint wallJumps = 0;
	uint maxWallJumps = 3;
	bool checkedFrame = false;

	float moveSpeed;
	float jumpSpeed;
	float knockbackSpeed;
	int numJumps;

	float stateTimer = 0.f;

	WEAPON_ID equipped_atk;
}; 

struct AnimatedMob {
	TEXTURE_ASSET_ID run;
	TEXTURE_ASSET_ID jump;
	TEXTURE_ASSET_ID idle;
	TEXTURE_ASSET_ID crouch;
	TEXTURE_ASSET_ID crouch_walk;
	TEXTURE_ASSET_ID slide;
};

struct Health {
	int hp = 10;
};

struct Physics {
	vec2 velocity = { 0.f, 0.f };
	vec2 targetVelocity = { 0.f, 0.f };
	bool inAir = false;
	bool onWall = false;
	float elasticity = 0.f;
	float drag = 0.15;
	float rampSpeed = 1.0;
};

struct Gravity {
	float grav = 70.f * TILE_SIZE;
	float terminalVelocity = 1000.f;
};

struct Collider {
	vec2 spr_scale = { 1.0f, 1.0f };
	vec2 offset = { 0.f, 0.f };
};

struct Solid {

};

struct Camera {
	vec2 targetPosition = { 0.f, 0.f };
	vec2 position = { 0.f, 0.f };
	vec2 dims = { 480, 270 };
	vec2 screenSize = { 1600, 900 };

	float defaultZoom = 1.0;
	float zoom = defaultZoom;
	float interpSpeed = 0.3;
	vec2 offset = { 16.f, 32.f };
};

enum class KEY {
	RIGHT = 0,
	LEFT,
	UP,
	DOWN,

	JUMP,

	BASIC, // i or z
	SPECIAL, // j or x
	GRAB, // k or c
	ENHANCE, // l or v
	DASH, // shift
	
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

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// COMBAT RELATED ENTITIES
struct Enemy {

};

enum class ATK_DIRL {
	TWO_WAY = 2,
	FOUR_WAY = 4,
	EIGHT_WAY = 8
};

POINT_DIRS getInputPointingDirection(Input& i, Motion& m, ATK_DIRL dirType);

enum class WEAPON_ID {
	NO_WEAPON = 0,
	CROWBAR,
	SHOTGUN,
	BASEBALL_BAT,
	BRASS_KNUCKLES,
	RAILGUN,
	POP_SICKLE,
	ARM_BLADES,
};

struct Weapon {
	int basic_jolt, special_jolt;
	float basic_jolt_time, special_jolt_time;
	int basic_kb, special_kb;
	int basic_dmg, special_dmg;
	int basic_range, special_range;
	ATK_DIRL basic_dirl, special_dirl;
};

struct DamageCollider {
	Entity source;
	vec2 knockback;
	int dmg;
	int ttl;
};

// INTERACTABLES
struct Interactable
{
	//  Anything that can be collided with by the player and picked up
	vec2 boundsScale = { 1.f, 1.f }; // scale of the hitbox to the size of the collider
	bool needsInput;
};

struct Tooltip
{
	std::string tooltipText = "";
};

struct Item
{
	ITEM_ID id;
};

// LEVEL COMPONENTS

struct Transition {
	// transitions need interactable
	LEVEL targetLevel;
	vec2 targetPosition;
};

struct TransitionData {
	// transitions need interactable
	LEVEL targetLevel;
	vec2 targetPosition;
	vec2 position;
	vec2 scale;
	bool needsInput;
	TEXTURE_ASSET_ID sprite = (TEXTURE_ASSET_ID) 0;
};

struct LevelElement {

};

struct Background {
	float parallaxDistance;
	vec2 centre;
};

struct BackgroundData {
	vec2 pos;
	vec2 scale;
	float parallaxDistance;
	TEXTURE_ASSET_ID background;
};

enum class LEVEL {
	TUT_INT_1,
	TUT_INT_2,
	TUT_INT_3,

	TUT_EXT_1,
	TUT_EXT_2,
	TUT_EXT_3,
	TUT_EXT_4,
	TUT_EXT_5,

	TUT_BRIDGE_1,
	TUT_BRIDGE_2,

	NUM_LEVELS,
};
const int num_levels = (int)LEVEL::NUM_LEVELS;

struct Level {
	std::string directory;
	vec2 mapPos;
	vec2 startPos;
	// backgrounds
	// doors
	std::vector<TransitionData> connects;
};

// END LEVEL COMPONENTS

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;

	int acc_frames = 0;

	float cur_ms_slow = 0.f, cur_ms_fast = 0.f;
	float redraw_slow = 500.f, redraw_fast = 42.f;
	int fps, px, py;
	float vx, vy;
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
	float counter_ms = 1000;
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
	DEFAULT = 0,
	NO_SPRITE,
	HITBOX,
	SOLID,
	SOLID_TILES,

	BREAKABLE_BOX,

	BG_TUT_INT_1,

	PLAYER,
	PLAYER_IDLE,
	PLAYER_IDLE_NOHELM,
	PLAYER_RUN,
	PLAYER_JUMP,
	GB_ENEMY,

	CROWBAR_ITEM,

	TEXTURE_COUNT
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	EGG,
	CHICKEN,
	TEXTURED,
	WIND,
	EFFECT_COUNT
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	CHICKEN = 0,
	SPRITE,
	EGG,
	DEBUG_LINE,
	SCREEN_TRIANGLE,
	GEOMETRY_COUNT
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

enum class RENDER_LAYER {
	BACKGROUND = 0,
	BG_DECOR,
	BG_ELEMENTS,
	ENTITIES,
	ITEMS,
	FG_DECOR,
	FOREGROUND,
	DEBUG
};

// All data relevant to the shape and motion of entities
struct Motion {
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 scale = { 10, 10 };

	bool visible;
	RENDER_LAYER render_layer;

	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

struct Animation
{
	TEXTURE_ASSET_ID sheet;
	int numFrames;
	int index;
	float frameRate;
	float msCounter = 0.f;

	std::vector<GLuint> vertex_buffers;
	GLuint indexBuffer; // we only need one index buffer for each frame i think?
};