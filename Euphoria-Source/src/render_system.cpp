// internal
#include "render_system.hpp"
#include <SDL.h>

#include <iomanip>
#include <sstream>

#include "tiny_ecs_registry.hpp"

void RenderSystem::drawTexturedMesh(Entity entity, 
									Motion& motion, 
									RenderRequest& render_request,
									const mat3 &projection)
{
	glBindVertexArray(vao);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.scale(motion.scale);
	// !!! TODO A1: add rotation to the chain of transformations, mind the order
	// of transformations

	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	GLuint vbo, ibo;
	// const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	// const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// check animation
	bool isAnimation = false;
	if (isAnimation = registry.animations.has(entity)) {
		Animation& anim = registry.animations.get(entity);

		vbo = anim.vertex_buffers[anim.index];
		ibo = anim.indexBuffer;
	} else {
		vbo = vertex_buffers[(GLuint)render_request.used_geometry];
		ibo = index_buffers[(GLuint)render_request.used_geometry];
	}

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void *)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		// assert(registry.renderRequests.has(entity));

		GLuint texture_id;

		if (isAnimation) {
			texture_id = 
				texture_gl_handles[(GLuint)registry.animations.get(entity).sheet];
		} else {
			texture_id =
				texture_gl_handles[(GLuint)render_request.used_texture];
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::CHICKEN || render_request.used_effect == EFFECT_ASSET_ID::EGG)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(ColoredVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(ColoredVertex), (void *)sizeof(vec3));
		gl_has_errors();

		if (render_request.used_effect == EFFECT_ASSET_ID::CHICKEN)
		{
			// Light up?
			GLint light_up_uloc = glGetUniformLocation(program, "light_up");
			assert(light_up_uloc >= 0);

			// !!! TODO A1: set the light_up shader variable using glUniform1i,
			// similar to the glUniform1f call below. The 1f or 1i specified the type, here a single int.
			gl_has_errors();
		}
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// wind
void RenderSystem::drawToScreen()
{
	// rebind vao bc fontinit unbinds it
	glBindVertexArray(vao);
	// Setting shaders
	// get the wind texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WIND]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint wind_program = effects[(GLuint)EFFECT_ASSET_ID::WIND];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(wind_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(wind_program, "darken_screen_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	ScreenState &screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.darken_screen_factor);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(wind_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw(float elapsed_ms)
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(0.9, 0.9, 0.9 , 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();
	mat3 projection_2D = createProjectionMatrix();
	// Draw all textured meshes that have a position and size component
	for (uint i = 0; i < registry.renderRequests.size(); ++i)
	{
		Entity entity = registry.renderRequests.entities[i];
		RenderRequest& rr = registry.renderRequests.components[i];

		if (!registry.motions.has(entity))
			continue;

		Motion& motion = registry.motions.get(entity);

		// check if motion in window
		Camera& cam = registry.cameras.components[0];
		if (motion.position.x > cam.position.x + (cam.dims.x / 2 + abs(motion.scale.x) / 2) ||
			motion.position.x < cam.position.x - (cam.dims.x / 2 + abs(motion.scale.x) / 2) ||
			motion.position.y > cam.position.y + (cam.dims.y / 2 + abs(motion.scale.y) / 2) ||
			motion.position.y < cam.position.y - (cam.dims.y / 2 + abs(motion.scale.y) / 2)) continue;

		// Note, its not very efficient to access elements indirectly via the entity
		// albeit iterating through all Sprites in sequence. A good point to optimize
		drawTexturedMesh(entity, motion, rr, projection_2D);
	}
	// Truely render to the screen
	drawToScreen(); 
	
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// RENDER TEXT HERE!!!!! because glfwSwapBuffers clears screen 
	
	if (debugging.in_debug_mode) {
		drawDebug(elapsed_ms);
	}


	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
}

void RenderSystem::drawDebug(float elapsed_ms) {
	debugging.cur_ms_fast += elapsed_ms;
	debugging.cur_ms_slow += elapsed_ms;
	debugging.acc_frames++;

	if (debugging.cur_ms_slow > debugging.redraw_slow) {
		debugging.fps = (int)(1000.f / debugging.cur_ms_slow * debugging.acc_frames);

		debugging.cur_ms_slow -= debugging.redraw_slow;
		debugging.acc_frames = 0;
	}

	if (debugging.cur_ms_fast > debugging.redraw_fast) {

		Entity& p = registry.players.entities[0];

		Motion& pm = registry.motions.get(p);
		debugging.px = pm.position.x;
		debugging.py = pm.position.y;

		Physics& ph = registry.physEntities.get(p);
		debugging.vx = ph.velocity.x;
		debugging.vy = ph.velocity.y;

		debugging.cur_ms_fast -= debugging.redraw_fast;
	}
	float sx = 16.f;
	float sy = window_height_px - 32.f;

	std::stringstream pos_str;
	pos_str << "Position: " << debugging.px << ", " << debugging.py;

	std::stringstream vel_str;
	vel_str << "Speed: " << std::fixed << std::setprecision(2) << debugging.vx << ", " << std::fixed << std::setprecision(2) << debugging.vy;

	std::stringstream dash_str;
	dash_str << "Dash duration: " << std::fixed << std::setprecision(2) << debugging.dashDuration << " ms";

	renderText("FPS: " + std::to_string(debugging.fps), sx, sy, 0.8f, { 1., 1., 1. }, glm::mat4(1.0f));
	sy -= 24.f;
	renderText(pos_str.str(), sx, sy, 0.8f, { 1., 1., 1. }, glm::mat4(1.0f));
	sy -= 24.f;
	renderText(vel_str.str(), sx, sy, 0.8f, { 1., 1., 1. }, glm::mat4(1.0f));
	sy -= 24.f;
	renderText(dash_str.str(), sx, sy, 0.8f, { 1., 1., 1. }, glm::mat4(1.0f));

}

mat3 RenderSystem::createProjectionMatrix()
{
	Entity player = registry.players.entities[0];
	auto& playerMotion = registry.motions.get(player);
	Camera& camera = registry.cameras.components[0];
	GameManager& gm = registry.gameManagers.components[0];

	camera.targetPosition = playerMotion.position;

	if (camera.zoom != camera.defaultZoom) {
		camera.zoom += 0.05 * (camera.defaultZoom - camera.zoom);
	}
	int xdir = (playerMotion.scale.x > 0) - (playerMotion.scale.x < 0);

	int halfWidth = camera.zoom * camera.dims.x / 2, halfHeight = camera.zoom * camera.dims.y / 2;

	camera.targetPosition.x += xdir * camera.offset.x;
	
	camera.targetPosition.y -= camera.offset.y;

	camera.targetPosition[0] = fmin(camera.targetPosition[0], gm.bounds[0] - halfWidth - 8);
	camera.targetPosition[0] = fmax(camera.targetPosition[0], halfWidth - 8);
	camera.targetPosition[1] = fmin(camera.targetPosition[1], gm.bounds[1] - halfHeight - 16);
	camera.targetPosition[1] = fmax(camera.targetPosition[1], halfHeight - 8);

	camera.position += camera.interpSpeed * (camera.targetPosition - camera.position);

	// Fake projection matrix, scales with respect to window coordinates
	float left = camera.position[0] - halfWidth;
	float top = camera.position[1] - halfHeight;

	gl_has_errors();
	float right = (float) camera.position[0] + halfWidth;
	float bottom = (float) camera.position[1] + halfHeight;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
}

void RenderSystem::renderText(const std::string& text, float x, float y,
	float scale, const glm::vec3& color,
	const glm::mat4& trans) {

	// activate the shaders!
	glUseProgram(m_font_shaderProgram);

	unsigned int textColor_location =
		glGetUniformLocation(
			m_font_shaderProgram,
			"textColor"
		);
	assert(textColor_location >= 0);
	glUniform3f(textColor_location, color.x, color.y, color.z);

	auto transform_location = glGetUniformLocation(
		m_font_shaderProgram,
		"transform"
	);
	assert(transform_location > -1);
	glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

	glBindVertexArray(m_font_VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_ftCharacters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// std::cout << "binding texture: " << ch.character << " = " << ch.TextureID << std::endl;

		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}