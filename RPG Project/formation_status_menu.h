#pragma once
#include <raylib.h>
#include "apply_ability_menu.h"

using namespace std;

class FormationStatusMenu : public ApplyAbilityMenu
{
public:
	virtual void setup(Game* game, PlayerStatusMenu* player_status_menu);
	virtual void update() override;
	virtual void draw() override;
private:
	//Setters
	void swapPartyMembers();
	void visuallyUpdateCursor();
	//Data
	int unit_num_1 = 0;
	bool selectedFirstUnit = false;
	bool selectedSecondUnit = false;
	bool flickerOn = false;
};