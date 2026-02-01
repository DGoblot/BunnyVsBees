#pragma once
#include "Entity.h"

class Drone : public Entity
{
public:
	Drone(int texx, int texy, float spawnx, float spawny);
	void update(float dt);
};

