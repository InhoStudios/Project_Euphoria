#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	// TODO: A1 add a LightUp component
	ComponentContainer<GameManager> gameManagers;
	ComponentContainer<Camera> cameras;
	ComponentContainer<Input> inputs;

	ComponentContainer<DeathTimer> deathTimers;

	ComponentContainer<Motion> motions;
	ComponentContainer<Physics> physEntities;
	ComponentContainer<Gravity> gravEntities;
	ComponentContainer <Solid> solids;
	ComponentContainer<Collider> colliders;

	ComponentContainer<LevelElement> levelElements;
	ComponentContainer<Collision> collisions;

	ComponentContainer<Interactable> interactables;
	ComponentContainer<Transition> transitions;

	ComponentContainer<Player> players;
	ComponentContainer<Mob> mobs;
	ComponentContainer<Health> healths;

	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<Animation> animations;
	ComponentContainer<ScreenState> screenStates;

	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		// TODO: A1 add a LightUp component
		registry_list.push_back(&gameManagers);
		registry_list.push_back(&cameras);
		registry_list.push_back(&inputs);

		registry_list.push_back(&deathTimers);

		registry_list.push_back(&motions);
		registry_list.push_back(&physEntities);
		registry_list.push_back(&gravEntities);
		registry_list.push_back(&solids);
		registry_list.push_back(&colliders);

		registry_list.push_back(&levelElements);
		registry_list.push_back(&collisions);

		registry_list.push_back(&interactables);
		registry_list.push_back(&transitions);

		registry_list.push_back(&players);
		registry_list.push_back(&mobs);
		registry_list.push_back(&healths);

		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&animations);
		registry_list.push_back(&screenStates);

		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;