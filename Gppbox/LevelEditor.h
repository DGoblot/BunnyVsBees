#pragma once

#include <string>
#include "SFML\System\Vector2.hpp"

class LevelEditor {
public:
	bool onoff = false;

	std::string mode = "wall";

	void AddWall(sf::Vector2i mousePos);
	void AddEmpty(sf::Vector2i mousePos);
	void SetPlayerSpawn(sf::Vector2i mousePos);
	void AddEnnemie(sf::Vector2i mousePos);

	void SaveToFile();
	void ReadFile();

	void im();
};