#pragma once
#include "Entity.h"
#include "C.hpp"
#include "Drone.h"

class Player : public Entity
{
public:
	float shootingCooldown = 0.1;
	float shootingTimer = 0.0;
	float recoil = 0.7;
	bool dead = false;

	Drone* drone;
	sf::Vector2f dronePosGoal;
	sf::Vector2f dronePosition;

	Player(int texx, int texy, float spawnx = 0.0, float spawny = 0.0);
	void update(float dt);
	void shoot();
	void im();
};

