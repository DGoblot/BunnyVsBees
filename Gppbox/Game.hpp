#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "sys.hpp"

#include "Particle.hpp"
#include "ParticleMan.hpp"

#include "Player.h"
#include "Ennemie.h"
#include "Bullet.h"

#include "LevelEditor.h"

using namespace sf;

class Entity;
class HotReloadShader;
class Game {
public:
	sf::RenderWindow*				win = nullptr;
	sf::Vector2f cameraPosition;
	sf::Vector2f cameraGoal;
	sf::Vector2f offset;
	float cameraLerpSpeed = 16.0f;
	float shakeStrength = 0.0f;
	float shakeMax = 17.0f;
	float shakeFade = 6.0f;

	LevelEditor levelEditor;

	sf::RectangleShape				bg;
	HotReloadShader *				bgShader = nullptr;

	sf::Texture						tex;

	bool							closing = false;
	
	std::vector<sf::Vector2i>		walls;
	std::vector<sf::Sprite> wallSprites;

	sf::Texture spriteSheet;

	ParticleMan beforeParts;
	ParticleMan afterParts;

	Player* player;
	std::vector<Ennemie*> ennemies;
	std::vector<Bullet*> bullets;


	static Game* g;


	Game(sf::RenderWindow * win);

	void cacheWalls();

	void processInput(sf::Event ev);
	bool wasSpacePressed = false;
	void pollInput(double dt);

	void update(double dt);

	void draw(sf::RenderWindow& win);

	bool isWall(int cx, int cy);
	void im();
	bool hasCollision(float gridx, float gridy);

};