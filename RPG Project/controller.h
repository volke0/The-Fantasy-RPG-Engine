#pragma once
#include <raylib.h>
#include <memory>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <json/json.h>

using namespace std;

class Party;
class Game;
class Maps;
class Level;

class Controller
{
public:
	virtual void update();

	//Pointers
	Level* level;
	Party* party;
	Game* game;
	Maps* map;
private:
};