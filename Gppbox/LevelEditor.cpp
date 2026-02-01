#include "LevelEditor.h"
#include "imgui.h"
#include "Game.hpp"

#include <iostream>
#include <fstream>

void LevelEditor::AddWall(sf::Vector2i mousePos) {
	Game& game = *Game::g;
	if (!(std::find(game.walls.begin(), game.walls.end(), mousePos) != game.walls.end())) {
		game.walls.push_back(mousePos);
		game.cacheWalls();
	}
}

void LevelEditor::AddEmpty(sf::Vector2i mousePos) {
	Game& game = *Game::g;
	if (std::find(game.walls.begin(), game.walls.end(), mousePos) != game.walls.end()) {
		game.walls.erase(std::remove(game.walls.begin(), game.walls.end(), mousePos), game.walls.end());
		game.cacheWalls();
	}
	bool found = false;
	for (auto e = game.ennemies.begin(); e != game.ennemies.end(); ) {
		if ((*e)->cx == mousePos.x && (*e)->cy == mousePos.y) {
			game.ennemies.erase(e);
			found = true;
			break;
		}
		++e;
	}
}

void LevelEditor::SetPlayerSpawn(sf::Vector2i mousePos) {
	Game& game = *Game::g;
	game.player->setPositionCell(mousePos.x + 0.5,mousePos.y + +0.5);
	game.player->dead = false;
	game.player->sprite.setColor(sf::Color(255, 255, 255, 255));
	game.player->sprite.setRotation(0);
}

void LevelEditor::AddEnnemie(sf::Vector2i mousePos) {
	Game& game = *Game::g;
	Ennemie* new_e = new Ennemie(1, 5, mousePos.x + 0.5, mousePos.y + 0.5);
	bool found = false;
	for (auto e = game.ennemies.begin(); e != game.ennemies.end(); ) {
		if ((*e)->cx == new_e->cx && (*e)->cy == new_e->cy) {
			delete new_e;
			found = true;
			break;
		}
		++e;
	}
	if (!found) {
		game.ennemies.push_back(new_e);
	}
}

void LevelEditor::SaveToFile() {
	ofstream levelFile("level.txt");
	Game& game = *Game::g;
	levelFile << "Walls" << endl;
	for (Vector2i& w : game.walls) {
		levelFile << w.x << " " << w.y << endl;;
	}
	levelFile << "Ennemies" << endl;
	for (auto e = game.ennemies.begin(); e != game.ennemies.end(); ) {
		levelFile << (*e)->cx << " " << (*e)->cy << endl;
		++e;
	}
	levelFile << "Player" << endl;
	levelFile << game.player->cx << " " << game.player->cy << endl;

	levelFile.close();
}

void LevelEditor::ReadFile() {
	ifstream levelFile("level.txt");
	string str;
	Game& game = *Game::g;
	int type = 0; // 0 is walls, 1 is ennemies, 2 is player
	while (getline(levelFile, str)) {
		if (str == "Walls") {
			type = 0;
			continue;
		}
		if (str == "Ennemies") {
			type = 1;
			continue;
		}
		if (str == "Player") {
			type = 2;
			continue;
		}
		int x = stoi(str.substr(0, str.find(" ")));
		int y = stoi(str.substr(str.find(" "), str.size()));
		
		if (type == 0) {
			game.walls.push_back(Vector2i(x, y));
			continue;
		}
		if (type == 1) {
			Ennemie* new_e = new Ennemie(1, 5, x + 0.5, y + 0.5);
			game.ennemies.push_back(new_e);
			continue;
		}
		if (type == 2) {
			game.player->setPositionCell(x, y);
			continue;
		}

	}

}

void LevelEditor::im() {
	using namespace ImGui;

	if (TreeNode("Level Editor")) {
		if (Button("Save Level")) {
			SaveToFile();
		}
		if (Button("Empty")) {
			mode = "empty";
		}
		if (Button("Wall")) {
			mode = "wall";
		}
		if (Button("Ennemie")) {
			mode = "ennemie";
		}
		if (Button("Player")) {
			mode = "player";
		}

		TreePop();

	}
}