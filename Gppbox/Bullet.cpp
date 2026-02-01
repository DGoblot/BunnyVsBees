#include "Bullet.h"
#include "Game.hpp"
#include <cmath>

Bullet::Bullet(int texx, int texy, float spawnx, float spawny) :
	Entity(texx, texy, spawnx, spawny) {
	sprite.setScale(sprite.getScale().x*0.6, sprite.getScale().y * 0.6);
	float angle = - shootAngle + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * shootAngle)));

	sprite.rotate(angle);

	angle = angle * 4.0 * atan(1.0) / 180.0;

	dx = 30.0f;
	dy = dx * sin(angle);
	dx = dx * cos(angle);
	gravity = 0.0f;
	friction = 0.0f;
	animCooldown = 0.016f;
	radius = 0.25f;
}

void Bullet::update(float dt) {
	//Bon c'est degeu mais pour changer une valeur, j'ai copié tout Entity::Update oups (techniquement c'est pas moins performant ehehe)

	Game& game = *Game::g; //grab Game singleton

	dy += gravity * dt;
	dy = clamp(dy, -1000.0f, 150.0f);

	dx *= 1 - clamp(dt * friction, 0.0f, 1.0f);

	rx += dx * dt; //movement
	ry += dy * dt;

	if (game.hasCollision(cx + 1, cy) && rx >= 0.7) {
		rx = 0.7;
		dx = 0.0;
		hitWall = true;
	}

	if (game.hasCollision(cx - 1, cy) && rx <= 0.3) {
		rx = 0.3;
		dx = 0.0;
		hitWall = true;
	}

	if (game.hasCollision(cx, cy + 1) && ry >= 0.9) {
		ry = 0.9;
		dy = 0.0;
		jumping = false;
	}

	if (game.hasCollision(cx, cy - 1) && ry <= 0.3) {
		ry = 0.3;
		dy = 0.0;
	}



	while (rx > 1.0) {
		rx--;
		cx++;
	}
	while (rx < 0.0) {
		rx++;
		cx--;
	}
	while (ry > 1.0) {
		ry--;
		cy++;
	}
	while (ry < 0.0) {
		ry++;
		cy--;
	}

	xx = cx + rx;
	yy = cy + ry;

	syncPixel();

	if (!flashDone) {
		animTimer += dt;
		if (animTimer >= animCooldown) {
			sprite.setTextureRect(sf::IntRect((texx + 1) * 16, texy * 16, 16, 16));
			flashDone = true;
		}
	}
	if (dx == 0.0 || dy == 0.0) {
		toDelete = true;
	}
	for (auto e = game.ennemies.begin(); e != game.ennemies.end(); ) {
		if (overlaps(*e)) {
			if (lookingLeft) (*e)->dx += -(*e)->impact;
			else (*e)->dx += (*e)->impact;
			(*e)->dy = -(*e)->impact*2;
			(*e)->sprite.setColor(sf::Color(255, 0, 0, 255));
			(*e)->damage_color = true;
			(*e)->health -= 1;
			toDelete = true;
			break;
		}
		++e;
	}
}
