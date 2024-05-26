#include "interactable_system.hpp"
#include "physics_system.hpp"
#include <iostream>

void InteractableSystem::step() {
	// iterate through players
	// iterate through collisions
	auto& playerRegistry = registry.players;
	auto& interactableRegistry = registry.interactables;

	for (int ie = 0; ie < registry.players.size(); ++ie) {
		Entity& p = registry.players.entities[ie];
		Player& player = registry.players.components[ie];

		Input& input = registry.inputs.get(p);

		for (int ii = 0; ii < registry.interactables.size(); ++ii) {
			Entity& i = registry.interactables.entities[ii];
			Interactable& interactable = registry.interactables.components[ii];

			if (collides_at(p, i)) {
				if (!interactable.requiresInput || input.key_press[KEY::GRAB]) {
					doInteract(i); // pass through parameters?
				}
			}
		}
	}
}

void InteractableSystem::doInteract(Entity e) {
	// check if item, transition, etc
	if (registry.transitions.has(e)) {

	}
}