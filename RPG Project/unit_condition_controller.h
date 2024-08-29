#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "controller.h"

using namespace std;

class Game;
class Unit;

class UnitConditionController : public Controller
{
public:
	void basicSetup(Game* game, Unit* unit);
	bool findCondition(string condition);

	//Setters
	void setCondition(string condition);
	void eraseCondition(string condition);
	void eraseAllConditions();

	//Data
	Unit* unit;
	vector<string> conditions = {};
};
