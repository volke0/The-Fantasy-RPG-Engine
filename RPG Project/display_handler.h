#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include "handler.h"

using namespace std;

class Game;
class BattleAnimationManager;
class Battle;
class Unit;

class DisplayHandler : public Handler
{
public:
	void basicSetup(Game* game, BattleAnimationManager* btl_animation_manager);
	virtual void update();
	
	//Data
	Game* game;
	BattleAnimationManager* btl_animation_manager;
	Battle* battle;
	Unit* user;
	Unit* current_target;
	string current_weapon;
};
