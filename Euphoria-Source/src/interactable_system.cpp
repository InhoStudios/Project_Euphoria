#include "interactable_system.hpp"

void InteractableSystem::step() {
	// iterate through players
	// iterate through collisions
	auto& playerRegistry = registry.players;
	auto& interactableRegistry = registry.interactables;

	for (int ie = 0; ie < registry.players.size(); ++ie) {
		Entity& p = registry.players.entities[ie];
		Player& player = registry.players.components[ie];

		for (int ii = 0; ii < registry.interactables.size(); ++ii) {
			Entity& i = registry.interactables.entities[ii];
			Interactable& interactable = registry.interactables.components[ii];
		}
	}
}

void InteractableSystem::doInteract() {

}