#pragma once
#include "Entity.h"

class Ennemie : public Entity
{
public:
	float impact = 2.0;
	bool damage_color = false;
	float damage_timer = 0.0;
	float damage_cooldown = 0.1;

	Ennemie(float texx, float texy, float spawnx = 0.0, float spawny = 0.0);
	void update(float dt);
	void im();
};

