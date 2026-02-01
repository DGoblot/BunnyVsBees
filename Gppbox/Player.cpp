#include "Player.h"
#include "imgui.h"
#include "Game.hpp"
#include "Bullet.h"
#include "Interp.hpp"

Player::Player(int texx, int texy, float spawnx, float spawny) :
	Entity(texx, texy, spawnx, spawny) {
	drone = new Drone(3, 5, spawnx - 1.0, spawny - 1.0);
	radius = 0.4f;
}

void Player::update(float dt) {
	Entity::update(dt);
	drone->update(dt);

	if (dead) return;
	
	dronePosGoal = Vector2f(xx * C::GRID_SIZE, yy * C::GRID_SIZE) - Vector2f(C::GRID_SIZE, C::GRID_SIZE);
	dronePosition = Interp::lerp(dronePosition, dronePosGoal, 15.0 * (float)dt);
	drone->setPositionPixel(dronePosition.x, dronePosition.y);

	shootingTimer += dt;
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
	else if (jumping) {
		launchJumpSprite = true;
	}
	else {
		launchIdleSprite = true;
	}
	if (launchJumpSprite) {
		sprite.setTextureRect(sf::IntRect((texx + 1) * 16, texy * 16, 16, 16));
		launchJumpSprite = false;
	}
	if (launchIdleSprite) {
		sprite.setTextureRect(sf::IntRect(texx * 16, texy * 16, 16, 16));
		launchIdleSprite = false;
	}
}

void Player::shoot() {
	if (shootingTimer >= shootingCooldown) {
		Game& game = *Game::g;
		Bullet* b = new Bullet(3, 4, xx + 0.5f, yy+0.2f);
		if (lookingLeft) {
			b->lookingLeft = true;
			b->dx *= -1;
			b->sprite.setScale(-b->sprite.getScale().x, b->sprite.getScale().y);
			b->setPositionCell(xx - 0.5f, yy + 0.2f);
			dx += recoil;
		}
		else dx -= recoil;

		game.shakeStrength = game.shakeMax;

		game.bullets.push_back(b);
		shootingTimer = 0.0;

	}
	
}

void Player::im() {
	using namespace ImGui;

	if (TreeNode("player")) {
		Value("pix x", sprite.getPosition().x);
		SameLine();
		Value("pix y", sprite.getPosition().y);
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
		DragFloat("Gravity", &this->gravity, 1.0, 0.0);
		DragFloat("Jump Speed", &this->jumpSpeed, 1.0, 0.0);
		//SameLine();
		DragFloat("Friction x", &this->friction, 0.1, 0.0);
		DragFloat("Recoil", &this->recoil, 0.01, 0.0, 100.0);


		TreePop();

	}
}