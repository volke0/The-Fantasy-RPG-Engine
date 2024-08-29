#pragma once
#include <raylib.h>
#include "controller.h"

using namespace std;

class Game;
class Unit;

class UnitBattleLogicController : public Controller
{
public:
	void setup(Game* game, Unit* unit);
	virtual void update() override;

	//Setters
	void setMagicMode();
	void setItemMode();
	void SetAttackMode();

	void setMagicDone();

	void endMagicMode();
	void endItemMode();
	void EndAttackMode();
	void EndHurtMode();

	void SetTarget(Unit* target);
	void SetCurrentWeapon(string current_weapon);

	void SetMeleeHurtMode();

	//Logic Variables
	bool play_mag = false;
	bool mag_done = false;
	bool item_done = false;
	bool play_item = false;
	bool play_atk = false;
	bool atk_done = false;
	bool hurt_done = false;
private:
	//Logic Functions
	void updateLogic();
	void updateMagic();
	void updateItem();
	void UpdateAttack();
	void UpdateMeleeHurt();

	//Data
	Unit* unit = nullptr;
	Unit* target = nullptr;
	string current_weapon = "";
};