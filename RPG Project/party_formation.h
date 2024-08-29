#pragma once
#include <raylib.h>
#include "formation_base.h"

using namespace std;

class Game;
class PartyBase;

class PartyFormation : public FormationBase
{
public:
	void setup(Game* game, PartyBase* party);
	virtual void update() override;
	virtual void draw() override;

	//Setters
	void SetFormation(string formation_name = "line_formation");
	void SwapMembers(int first_unit_pos, int second_unit_pos);
private:
	//Setup
	void parseScript(string formation_name = "line_formation");

	//Data
	PartyBase* party;
};