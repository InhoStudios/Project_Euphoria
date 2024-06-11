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

	Weapon none, crowbar, shotgun, baseball_bat, brass_knuckles;

	WeaponRegistry() {
		weapons = {
			&none,
			&crowbar
		};

		initWeapons();
	}
private:

	void initWeapons() {
		none = {
			75, 100,
			1, 1,
			1, 2,
			24, 16,
			ATK_DIRL::TWO_WAY, ATK_DIRL::TWO_WAY
		};

		crowbar = {
			75, 100,
			1, 1,
			1, 2,
			24, 16,
			ATK_DIRL::TWO_WAY, ATK_DIRL::TWO_WAY
		};
	}
};

extern WeaponRegistry weapon;