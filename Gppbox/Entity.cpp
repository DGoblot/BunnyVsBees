#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "C.hpp"
#include "imgui.h"
#include "Game.hpp"
#include "stdlib.h"

Entity::Entity(float texx, float texy, float spawnx, float spawny) {
	Game& game = *Game::g;
	this->texx = texx;
	this->texy = texy;
	sprite.setTexture(game.spriteSheet);
	sprite.setTextureRect(sf::IntRect(texx*16, texy*16, 16, 16));
	sprite.setOrigin(sf::Vector2f(8.0f, 8.0f));
	sprite.setScale(sf::Vector2f(C::PIXEL_SCALE, C::PIXEL_SCALE));
	setPositionCell(spawnx, spawny);
}

void Entity::setPositionPixel(float px, float py) {
	sprite.setPosition({ px,py });

	cx = (int)px / C::GRID_SIZE;
	rx = ((1.0f * px / C::GRID_SIZE) - cx);
	cy = (int)py / C::GRID_SIZE;
	ry = ((1.0f * py / C::GRID_SIZE) - cy);
	xx = cx + rx;
	yy = cy + ry;
}

void Entity::setPositionCell(float cx, float cy) {
	int pixx = cx * C::GRID_SIZE;
	int pixy = cy * C::GRID_SIZE;
	this->cx = (int)cx;
	this->cy = (int)cy;
	this->rx = cx - (int)cx;
	this->ry = cy - (int)cy;
	xx = cx + rx;
	yy = cy + ry;

	sprite.setPosition(pixx, pixy);
}

void Entity::update(float dt) {

	Game& game = *Game::g; //grab Game singleton

	dy += gravity *dt;
	dy = clamp(dy, -1000.0f, 50.0f);

	dx *= 1 - clamp(dt * friction, 0.0f, 1.0f);

	rx += dx * dt; //movement
	ry += dy * dt;
	
	if (game.hasCollision(cx + 1, cy)  && rx >= 0.7) {
		rx = 0.7;
		dx = 0.0;
		hitWall = true;
	}

	if (game.hasCollision(cx - 1, cy) && rx <= 0.3) {
		rx = 0.3;
		dx = 0.0;
		hitWall = true;
	}
	
	if (game.hasCollision(cx, cy + 1) && ry >= 0.5) {
		ry = 0.5;
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

}

bool Entity::overlaps(Entity* e) {
	float maxDist = radius + e->radius;
	// classic distance formula
	float distSqr = (e->xx - xx) * (e->xx - xx) + (e->yy - yy) * (e->yy - yy);
	return distSqr <= maxDist * maxDist;
}


void Entity::syncPixel() {
	int pixx = (cx + rx) * C::GRID_SIZE;
	int pixy = (cy + ry) * C::GRID_SIZE;
	sprite.setPosition(pixx, pixy);
}

void Entity::draw(sf::RenderWindow& win) {
	try
	{
		win.draw(sprite);
	}
	catch (const std::exception&)
	{

	}
	
}