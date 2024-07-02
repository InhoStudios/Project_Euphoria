#pragma once

#include <array>
#include <utility>
#include <iostream>

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"

// fonts
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
// matrices
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
	char character;
};

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  // std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::CHICKEN, mesh_path("chicken.obj"))
		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("spr_default_item.png"),
			textures_path("spr_no_sprite.png"),
			textures_path("spr_hitbox.png"),
			textures_path("spr_solid.png"),
			textures_path("spr_solid_tiles.png"),

			textures_path("spr_breakable_box.png"),

			level_path("tut_int_1/background.png"),

			textures_path("spr_player.png"),
			textures_path("player/spr_player_idle.png"),
			textures_path("player/spr_player_idle_nohelm.png"),
			textures_path("player/spr_player_run.png"),
			textures_path("player/spr_player_jump.png"),

			textures_path("player/spr_player_idle_crowbar.png"),
			textures_path("player/spr_player_run_crowbar.png"),
			textures_path("player/spr_player_jump_crowbar.png"),
			textures_path("player/spr_player_crowbar_swing.png"),

			textures_path("spr_enemy.png"),

			textures_path("spr_crowbar_item.png"),
	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("chicken"),
		shader_path("textured"),
		shader_path("wind") };

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the wind
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw(float elapsed_ms);

	mat3 createProjectionMatrix();

	bool initFont(const std::string& font_filename, unsigned int font_default_size);
	void renderText(const std::string& text, float x, float y, float scale, const glm::vec3& color, const glm::mat4& trans);

private:
	// Internal drawing functions for each entity type
	// void drawTexturedMesh(Entity entity, const mat3& projection);
	void stepBackgrounds();

	bool outOfView(Motion& motion);

	void drawTexturedMesh(Entity entity, Motion& motion, const mat3& projection);
	void drawToScreen();

	void drawDebug(float elapsed_ms);

	void drawTooltips();

	vec2 worldToScreenSpace(vec2 world_space);
	vec2 centerText(std::string text, float scale, vec2 coords);

	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;

	//vao
	GLuint vao;

	// fonts
	std::map<char, Character> m_ftCharacters;
	GLuint m_font_shaderProgram;
	GLuint m_font_VAO;
	GLuint m_font_VBO;

	GLuint m_screen_shaderProgram;
	GLuint m_screen_VAO;
	GLuint m_screen_VBO;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);
