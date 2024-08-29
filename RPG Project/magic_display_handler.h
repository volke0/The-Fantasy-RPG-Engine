#pragma once
#include <raylib.h>
#include "display_handler.h"

using namespace std;

class Game;
class BattleAnimationManager;
class Magic;

class MagicDisplayHandler : public DisplayHandler
{
public:
	void setup(Game* game, BattleAnimationManager* btl_animation_manager);
	void update() override;
	void draw();

	//Setters
	void setMagicList(vector<shared_ptr<Magic>> magic_list);
private:
	//Update
	void displayMagic();

	//Setters
	void setBattleVars();
	void setCurrentTarget();
	void setCurrentMagic();

	//Magic
	vector<shared_ptr<Magic>> magic_list;
	int mag_index = 0;
	shared_ptr<Magic> current_magic;

	//Logic
	bool play_mag = false;
	bool applied_dmg = false;
	bool magic_mode_on = false;
};
