#pragma once
#include "SFML/Graphics.hpp"

class Entity {
public :
	bool toDelete = false;
	int health = 0.0;
	bool lookingLeft = false;
	bool changedDirection = false;


	// Graphical object
	sf::Sprite sprite;
	int texx = 0;
	int texy = 0;
	float animTimer = 0.0f;
	float animCooldown = 0.1f;
	int frame = 0;
	bool launchJumpSprite = false;
	bool launchIdleSprite = false;

	// Base coordinates
	int cx = 3;
	int cy = 3;
	float rx = 0.5;
	float ry = 0.5;

	// Resulting coordinates
	float xx = cx + rx;
	float yy = cy + ry;

	// Movements
	float dx = 0.0;
	float dy = 0.0;
	float speed = 150.0;
	float gravity = 80.0;
	float friction = 11.0;

	float jumpSpeed = 20.0;
	bool jumping = false;

	// Collisions
	bool hitWall = false;

	float radius;

	Entity(float texx, float texy, float spawnx = 0.0, float spawny = 0.0);

	void setPositionPixel(float px, float py);
	void setPositionCell(float cx, float cy);

	void update(float dt);
	bool overlaps(Entity* e);

	void im();
	void syncPixel();

	void draw(sf::RenderWindow& win);
};
