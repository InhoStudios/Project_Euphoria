#pragma once

#include "tiny_ecs_registry.hpp"
#include "common.hpp"

Entity instantiateDamage(Entity source, vec2 position, vec2 scale, 
	float knockback, int damage, int ttl);

class CombatSystem
{
public:
	CombatSystem() { }

	void step();
private:

};

class WeaponRegistry
{
public:
	std::vector<Weapon*> weapons;

	Weapon crowbar, shotgun, baseball_bat, brass_knuckles;

	WeaponRegistry() {

		weapons = {

		};
	}
private:
	void initWeapons() {
		crowbar = {
			5, 15,
			50, 50,
			1, 2,
			8, 16,
			ATK_DIRL::TWO_WAY, ATK_DIRL::TWO_WAY
		};
	}
};

extern WeaponRegistry weapon;