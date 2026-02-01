#include "Drone.h"

Drone::Drone(int texx, int texy, float spawnx, float spawny) :
	Entity(texx, texy, spawnx, spawny) {
	animCooldown = 0.5;
	gravity = 0.0f;
	friction = 0.0f;
	sprite.setScale(sprite.getScale().x * 0.7, sprite.getScale().y * 0.7);
}

void Drone::update(float dt) {
	this->Entity::update(dt);

	animTimer += dt;
	if (animTimer >= animCooldown) {
		switch (frame)
		{
		case 0:
			sprite.setTextureRect(sf::IntRect((texx + 1) * 16, texy * 16, 16, 16));
			frame = 1;
			break;
		case 1:
			sprite.setTextureRect(sf::IntRect(texx * 16, texy * 16, 16, 16));
			frame = 0;
			break;
		}
		animTimer = 0.0f;
	}
}
