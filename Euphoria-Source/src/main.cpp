
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "animation_system.hpp"
#include "interactable_system.hpp"
#include "ai_system.hpp"
#include "combat_system.hpp"

using Clock = std::chrono::high_resolution_clock;

// Entry point
int main()
{
	// Global systems
	WorldSystem world;
	RenderSystem renderer;
	PhysicsSystem physics;
	AnimationSystem animations;
	AISystem ai;
	InteractableSystem interactables;
	CombatSystem combat;

	// Initializing window
	GLFWwindow* window = world.create_window();
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	renderer.init(window);
	world.init(&renderer);

	// variable timestep loop
	auto t = Clock::now();
	while (!world.is_over()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;

		float clamped_ms = min(elapsed_ms, 16.6666667f);

		physics.checkCollisions();

		ai.step(clamped_ms);

		world.step(clamped_ms);

		physics.step(clamped_ms);

		combat.step(clamped_ms);

		interactables.step();

		world.clear_keys();

		animations.step(clamped_ms);
		renderer.draw(elapsed_ms);
	}

	return EXIT_SUCCESS;
}
