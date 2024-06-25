#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
	float knockback, int damage, int ttl);

class CombatSystem
{
public:
	CombatSystem() { }

	void step(float elapsed_ms);
private:

};

class WeaponRegistry
{
public:
	std::vector<Weapon*> weapons;

	Weapon none, crowbar, shotgun, baseball_bat, brass_knuckles, railgun, pop_sickle, arm_blades;

	WeaponRegistry() {
		weapons = {
			&none,
			&crowbar,
			&shotgun,
			&baseball_bat,
			&brass_knuckles,
			&railgun,
			&pop_sickle,
			&arm_blades
		};

		initWeapons();
	}
private:

	void initWeapons() {
		none = {
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			0, 0,
			ATK_DIRL::TWO_WAY, ATK_DIRL::TWO_WAY
		};

		crowbar = {
			75, 100,
			100, 100,
			1, 1,
			1, 2,
			24, 24,
			ATK_DIRL::TWO_WAY, ATK_DIRL::TWO_WAY
		};


		shotgun = {
			-300, -450,
			120, 120,
			1, 1,
			1, 2,
			0, 0,
			ATK_DIRL::FOUR_WAY, ATK_DIRL::FOUR_WAY
		};
	}
};

extern WeaponRegistry weapon;