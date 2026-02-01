#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
	bool flashDone = false;
	float shootAngle = 5.0f;

	Bullet(int texx, int texy, float spawnx, float spawny);
	void update(float dt);
};

