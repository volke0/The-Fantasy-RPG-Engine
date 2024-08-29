#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

class Game;

class Entity
{
public:
	//Setup
	void setupGame(Game* game);

	//Data
	Game* game;
	string name;
private:
};

