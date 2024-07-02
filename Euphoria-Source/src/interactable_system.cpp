#include "interactable_system.hpp"
#include "physics_system.hpp"
#include <iostream>
#include "level_registry.hpp"

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
				if (!interactable.needsInput || input.key_press[KEY::GRAB]) {
					doInteract(i); // pass through parameters?
				}
			}
		}
	}
}

void InteractableSystem::doInteract(Entity e) {
	// check if item, transition, etc
	if (registry.transitions.has(e)) {
		Transition& to = registry.transitions.get(e);
		transitionTo(to);
	}
	else if (registry.items.has(e)) {
		Item& i = registry.items.get(e);
		registry.gameManagers.components[0].inventory[(int) i.id]++;
		switch (i.id) {
		case ITEM_ID::CROWBAR:
			registry.mobs.get(registry.players.entities[0]).equipped_atk = WEAPON_ID::CROWBAR;
			break;
		}
		registry.remove_all_components_of(e);
	}
}