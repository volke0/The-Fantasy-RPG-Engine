#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include "controller.h"

using namespace std;

class Game;
class Unit;

class StateController : public Controller
{
public:
	void basicSetup(Game* game);
	void update() override;

	void pushState(string state);
	void eraseState(string state);
	void clearStates();

	//Getters
	bool findState(string key);
	bool findLastState(string key);

	//Data
	Game* game;
	vector<string> states = {};
};
