#pragma once
#include <raylib.h>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <json/json.h>

using namespace std;

class Game;
class Unit;

class AbilityBase
{
public:
	void basicSetup(Game* game);

	//Data
	Game* game;
};
