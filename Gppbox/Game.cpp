
#include <iostream>
#include <imgui.h>
#include <array>
#include <vector>
#include <random>


#include "C.hpp"
#include "Game.hpp"
#include "Player.h"
#include "Interp.hpp"

#include "HotReloadShader.hpp"

Game* Game::g = 0;
static int lastCol = C::RES_X / C::GRID_SIZE - 1;
static int lastLine = C::RES_Y / C::GRID_SIZE;

Game::Game(sf::RenderWindow * win) {
	this->win = win;

	g = this;

	if (!spriteSheet.loadFromFile("res/tilemap_packed.png"))
	{
		// erreur...
	}

	bg = sf::RectangleShape(Vector2f((float)win->getSize().x, (float)win->getSize().y));

	bool isOk = tex.loadFromFile("res/backgroundColorForest.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(C::RES_X, C::RES_Y));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	/*for (int i = 0; i < C::RES_X / C::GRID_SIZE; ++i)
	{
		walls.push_back(Vector2i(i, lastLine));
		walls.push_back(Vector2i(i, lastLine-1));
	}

	for (int i = 0; i < C::RES_Y / C::GRID_SIZE; ++i)
	{
		walls.push_back(Vector2i(0, i));
		walls.push_back(Vector2i(1, i));
	}

	for (int i = 0; i < C::RES_Y / C::GRID_SIZE; ++i)
	{
		walls.push_back(Vector2i(lastCol, i));
		walls.push_back(Vector2i(lastCol-1, i));
	}*/

	player = new Player(0, 4, 5, 5);
	levelEditor.ReadFile();
	cacheWalls();

}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		int wall_sprite = 0;
		sf::Sprite sprite;
		sprite.setTexture(spriteSheet);
		if ((std::find(walls.begin(), walls.end(), w + Vector2i(0, -1)) != walls.end())) {
			sprite.setTextureRect(sf::IntRect(4 * 16, 1 * 16, 16, 16));
		}
		else {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			if (r <= 0.25) sprite.setTextureRect(sf::IntRect(4 * 16, 0 * 16, 16, 16));
			else {
				if (r <= 0.5) sprite.setTextureRect(sf::IntRect(0 * 16, 2 * 16, 16, 16));
				else {
					if (r <= 0.75) sprite.setTextureRect(sf::IntRect(1 * 16, 2 * 16, 16, 16));
					else sprite.setTextureRect(sf::IntRect(2 * 16, 2 * 16, 16, 16));
				}
			}
			
		}
		
		
		sprite.setPosition((float)w.x * C::GRID_SIZE, (float)w.y * C::GRID_SIZE);
		sprite.setScale(sf::Vector2f(C::PIXEL_SCALE, C::PIXEL_SCALE));
		wallSprites.push_back(sprite);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		int here = 0;
		if (ev.key.code == Keyboard::K) {
			int there = 0;
			walls.clear();
			cacheWalls();
		}
		if (ev.key.code == Keyboard::Tab) {
			if (!levelEditor.onoff) levelEditor.onoff = true;
			else {
				levelEditor.onoff = false;
			}
			
		}
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;

	if (!levelEditor.onoff) {
		if (player->dead) return;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
			player->dx -= player->speed * dt;
			if (!player->lookingLeft) {
				player->lookingLeft = true;
				player->sprite.setScale(-player->sprite.getScale().x, player->sprite.getScale().y);
				player->drone->sprite.setScale(-player->drone->sprite.getScale().x, player->drone->sprite.getScale().y);
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			player->dx += player->speed * dt;
			if (player->lookingLeft) {
				player->lookingLeft = false;
				player->sprite.setScale(-player->sprite.getScale().x, player->sprite.getScale().y);
				player->drone->sprite.setScale(-player->drone->sprite.getScale().x, player->drone->sprite.getScale().y);
			}
			;
		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			player->shoot();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
			if (!wasSpacePressed) {
				if (!player->jumping) {
					player->dy -= player->jumpSpeed;
					player->jumping = true;
					wasSpacePressed = true;
				}
			}
		}
		else {
			wasSpacePressed = false;
		}
	}
	else {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (ImGui::GetIO().WantCaptureMouse) {
				return;
			}
			Vector2i click = Vector2i(floor(ImGui::GetMousePos().x), floor(ImGui::GetMousePos().y));
			Vector2i clickOnGrid = click / C::GRID_SIZE;
			if (clickOnGrid.x < 1 || clickOnGrid.x >(C::RES_X / C::GRID_SIZE) - 2) return;
			if (clickOnGrid.y < 1 || clickOnGrid.y >(C::RES_Y / C::GRID_SIZE) - 2) return;
			if (levelEditor.mode == "wall") {
				levelEditor.AddWall(clickOnGrid);		
			}
			else if (levelEditor.mode == "empty") {
				levelEditor.AddEmpty(clickOnGrid);
			}
			else if (levelEditor.mode == "ennemie") {
				levelEditor.AddEnnemie(clickOnGrid);
			}
			else if (levelEditor.mode == "player") {
				levelEditor.SetPlayerSpawn(clickOnGrid);
			}
				
		}
	}

}

static sf::VertexArray va;
static RenderStates vaRs;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);

	if (!levelEditor.onoff) {

		player->update(dt);

		for (auto e = ennemies.begin(); e != ennemies.end(); ) {
			if ((*e)->toDelete) {
				delete* e;
				e = ennemies.erase(e);
			}
			else {
				(*e)->dy += (*e)->gravity * dt;
				(*e)->update(dt);
				++e;
			}
		}

		for (auto b = bullets.begin(); b != bullets.end();) {
			if ((*b)->toDelete) {
				delete* b;
				b = bullets.erase(b);
			}
			else {
				(*b)->update(dt);
				++b;
			}
		}
	}

	if (!levelEditor.onoff) {

	//Camera handling (lerp & screenshake & switch between levelEdit and gameplay)

	
		if (shakeStrength > 0.01f) {
			shakeStrength = shakeStrength - (shakeStrength * shakeFade * dt);
			offset.x = -shakeStrength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * shakeStrength)));
			offset.y = -shakeStrength + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * shakeStrength)));
		}
		cameraGoal = Vector2f(player->xx * C::GRID_SIZE, player->yy * C::GRID_SIZE) + offset;
		cameraPosition = Interp::lerp(cameraPosition, cameraGoal, cameraLerpSpeed * (float)dt);

	}

	afterParts.update(dt);
}

void Game::draw(sf::RenderWindow& win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader* sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	/*for (sf::RectangleShape& r : rects)
		win.draw(r);*/

	for (sf::Sprite& s : wallSprites) {
		win.draw(s);
	}	
	
	player->draw(win);
	player->drone->draw(win);

	for (Ennemie* e : ennemies) {
		e->draw(win);
	}

	for (Bullet* b : bullets) {
		b->draw(win);
	}


	afterParts.draw(win);
}

 bool Game::hasCollision(float gridx, float gridy) {
	 if (gridx > (C::RES_X / C::GRID_SIZE) - 1 || gridx < 0.0) { //check for left and right border
		 return true;
	 }
	 if (gridy > (C::RES_Y / C::GRID_SIZE) - 1) { //check for left and right border
		 return true;
	 }
	 for (auto wall : walls) {
		 if (wall.x == (int)gridx && wall.y == (int)gridy) { //check for walls (loops through every wall)
			 return true;
		 }
	 }
	 return false;
}

bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{
	using namespace ImGui;

	levelEditor.im();

	if (TreeNode("game")) {
		DragFloat("ShakeMax", &this->shakeMax, 0.1, 0.0, 100.0f);
		DragFloat("ShakeFade", &this->shakeFade, 0.1, 0.0, 100.0f);
		DragFloat("LerpSpeed", &this->cameraLerpSpeed, 0.1, 0.0, 100.0f);
		TreePop();
	}
	
	player->im();
	if (!ennemies.empty()) ennemies[0]->im();
}

