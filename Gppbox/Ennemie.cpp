#include "Ennemie.h"
#include "imgui.h"
#include "Game.hpp"
#include <iostream>

Ennemie::Ennemie(float texx, float texy, float spawnx, float spawny) :
	Entity(texx, texy, spawnx, spawny) {
	health = 3.0;
	speed = 30.0;
	animCooldown = 0.4;
	radius = 0.3f;
}

void Ennemie::update(float dt) {
	Game& game = *Game::g; //grab Game singleton

	dx += speed * dt;
	this->Entity::update(dt);

	if (damage_color) {
		damage_timer += dt;
		if (damage_timer >= damage_cooldown) {
			sprite.setColor(sf::Color(255, 255, 255, 255));
			damage_color = false;
			damage_timer = 0.0;
		}
	}

	animTimer += dt;
	if ((dx > 1.0 || dx < -1.0) && !jumping) {
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

	if (hitWall) {
		speed *= -1;
		hitWall = false;
		sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
	}

	if (overlaps(game.player)) {
		game.player->dead = true;
		game.player->dy = -10.0;
		if (xx > game.player->xx) {
			game.player->dx = -10.0;
			game.player->sprite.setRotation(-90);
		}
		else {
			game.player->dx = 10.0;
			game.player->sprite.setRotation(90);
		}
		game.player->sprite.setColor(sf::Color(255, 0, 0, 255));
		game.player->sprite.setTextureRect(sf::IntRect(5 * 16, 4 * 16, 16, 16));
	}

	if (health <= 0.0) {
		toDelete = true;
	}

}

void Ennemie::im() {
	using namespace ImGui;

	if (TreeNode("ennemie")) {
		Value("xx", xx);
		SameLine();
		Value("yy", yy);
		Value("dx", dx);
		SameLine();
		Value("dy", dy);

		if (Button("Set Pos (100 100)")) {
			setPositionPixel(100, 100);
		}

		DragFloat("Speed", &this->speed, 1.0, 0.0);


		TreePop();

	}
}